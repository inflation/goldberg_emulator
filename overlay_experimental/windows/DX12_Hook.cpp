#include "DX12_Hook.h"
#include "Windows_Hook.h"
#include "../Renderer_Detector.h"
#include "../../dll/dll.h"

#ifndef NO_OVERLAY

#include <imgui.h>
#include <impls/windows/imgui_impl_dx12.h>

#include <dxgi1_4.h>

DX12_Hook* DX12_Hook::_inst = nullptr;

bool DX12_Hook::start_hook()
{
    bool res = true;
    if (!hooked)
    {
        if (!Windows_Hook::Inst()->start_hook())
            return false;

        PRINT_DEBUG("Hooked DirectX 12\n");
        hooked = true;

        Renderer_Detector::Inst().renderer_found(this);

        BeginHook();
        HookFuncs(
            std::make_pair<void**, void*>(&(PVOID&)DX12_Hook::Present, &DX12_Hook::MyPresent),
            std::make_pair<void**, void*>(&(PVOID&)DX12_Hook::ResizeTarget, &DX12_Hook::MyResizeTarget),
            std::make_pair<void**, void*>(&(PVOID&)DX12_Hook::ResizeBuffers, &DX12_Hook::MyResizeBuffers),
            std::make_pair<void**, void*>(&(PVOID&)DX12_Hook::ExecuteCommandLists, &DX12_Hook::MyExecuteCommandLists),
            std::make_pair<void**, void*>(&(PVOID&)DX12_Hook::Close, &DX12_Hook::MyClose)
        );
        EndHook();

        get_steam_client()->steam_overlay->HookReady();
    }
    return res;
}

void DX12_Hook::resetRenderState()
{
    if (initialized)
    {
        pSrvDescHeap->Release();

        ImGui_ImplDX12_Shutdown();
        Windows_Hook::Inst()->resetRenderState();
        ImGui::DestroyContext();

        initialized = false;
    }
}

// Try to make this function and overlay's proc as short as possible or it might affect game's fps.
void DX12_Hook::prepareForOverlay(IDXGISwapChain* pSwapChain)
{
    pSwapChain->GetDesc(&sc_desc);

    if (!initialized)
    {
        ID3D12Device* pDevice;
        if (pSwapChain->GetDevice(IID_PPV_ARGS(&pDevice)) != S_OK)
            return;

        {
            D3D12_DESCRIPTOR_HEAP_DESC desc = {};
            desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            desc.NumDescriptors = 1;
            desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            if (pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&pSrvDescHeap)) != S_OK)
            {
                pDevice->Release();
                return;
            }
        }

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = NULL;
    
        ImGui_ImplDX12_Init(pDevice, 1, DXGI_FORMAT_R8G8B8A8_UNORM,
            pSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
            pSrvDescHeap->GetGPUDescriptorHandleForHeapStart());
    
        pDevice->Release();

        get_steam_client()->steam_overlay->CreateFonts();

        initialized = true;
    }
}

HRESULT STDMETHODCALLTYPE DX12_Hook::MyPresent(IDXGISwapChain *_this, UINT SyncInterval, UINT Flags)
{
    DX12_Hook::Inst()->prepareForOverlay(_this);

    return (_this->*DX12_Hook::Inst()->Present)(SyncInterval, Flags);
}

HRESULT STDMETHODCALLTYPE DX12_Hook::MyResizeTarget(IDXGISwapChain* _this, const DXGI_MODE_DESC* pNewTargetParameters)
{
    DX12_Hook::Inst()->resetRenderState();
    return (_this->*DX12_Hook::Inst()->ResizeTarget)(pNewTargetParameters);
}

HRESULT STDMETHODCALLTYPE DX12_Hook::MyResizeBuffers(IDXGISwapChain* _this, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
    DX12_Hook::Inst()->resetRenderState();
    return (_this->*DX12_Hook::Inst()->ResizeBuffers)(BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

void STDMETHODCALLTYPE DX12_Hook::MyExecuteCommandLists(ID3D12CommandQueue *_this, UINT NumCommandLists, ID3D12CommandList* const* ppCommandLists)
{
    DX12_Hook* me = DX12_Hook::Inst();
    // ----------------------------------------------------- //
    // \/\/\/ TODO: Find a cleaner way to do all this \/\/\/ //
    //      I'd like to put it in IDXGISwapChain::Present    //
    // ----------------------------------------------------- //
    if (me->initialized)
    {
        static std::recursive_mutex render_mutex; // Sniper Elite 4 where I test this uses multiple thread on this. ImGui is not reentrant
        std::lock_guard<std::recursive_mutex> lock(render_mutex);
        for (int i = 0; i < NumCommandLists; ++i)
        {
            if (((ID3D12GraphicsCommandList*)ppCommandLists[i])->GetType() == D3D12_COMMAND_LIST_TYPE_DIRECT)
            {
                ImGui_ImplDX12_NewFrame();
                Windows_Hook::Inst()->prepareForOverlay(me->sc_desc.OutputWindow);

                ImGui::NewFrame();

                get_steam_client()->steam_overlay->OverlayProc();

                ((ID3D12GraphicsCommandList*)ppCommandLists[i])->SetDescriptorHeaps(1, &me->pSrvDescHeap);
                ImGui::Render();
                ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), (ID3D12GraphicsCommandList*)ppCommandLists[i]);
                (((ID3D12GraphicsCommandList*)ppCommandLists[i])->*me->Close)();
            }
        }
    }
    (_this->*DX12_Hook::Inst()->ExecuteCommandLists)(NumCommandLists, ppCommandLists);
}

HRESULT STDMETHODCALLTYPE DX12_Hook::MyClose(ID3D12GraphicsCommandList* _this)
{
    if (DX12_Hook::Inst()->initialized && _this->GetType() == D3D12_COMMAND_LIST_TYPE_DIRECT)
        return S_OK;
    else
        return (_this->*DX12_Hook::Inst()->Close)();
}

DX12_Hook::DX12_Hook():
    initialized(false),
    hooked(false),
    pSrvDescHeap(nullptr),
    Present(nullptr),
    ResizeBuffers(nullptr),
    ResizeTarget(nullptr),
    ExecuteCommandLists(nullptr),
    Close(nullptr)
{
    _library = LoadLibrary(DLL_NAME);

    PRINT_DEBUG("DX12 support is experimental, don't complain if it doesn't work as expected.\n");
}

DX12_Hook::~DX12_Hook()
{
    PRINT_DEBUG("DX12 Hook removed\n");

    if (initialized)
    {
        ImGui_ImplDX12_InvalidateDeviceObjects();
        ImGui::DestroyContext();

        initialized = false;
    }

    FreeLibrary(reinterpret_cast<HMODULE>(_library));

    _inst = nullptr;
}

DX12_Hook* DX12_Hook::Inst()
{
    if (_inst == nullptr)
        _inst = new DX12_Hook();

    return _inst;
}

const char* DX12_Hook::get_lib_name() const
{
    return DLL_NAME;
}

void DX12_Hook::loadFunctions(ID3D12CommandQueue* pCommandQueue, ID3D12GraphicsCommandList* pCommandList, IDXGISwapChain *pSwapChain)
{
    void** vTable;
    
    vTable = *reinterpret_cast<void***>(pCommandQueue);
#define LOAD_FUNC(X) (void*&)X = vTable[(int)ID3D12CommandQueueVTable::X]
    LOAD_FUNC(ExecuteCommandLists);
#undef LOAD_FUNC

    vTable = *reinterpret_cast<void***>(pCommandList);
#define LOAD_FUNC(X) (void*&)X = vTable[(int)ID3D12GraphicsCommandListVTable::X]
    LOAD_FUNC(Close);
#undef LOAD_FUNC

    vTable = *reinterpret_cast<void***>(pSwapChain);
#define LOAD_FUNC(X) (void*&)X = vTable[(int)IDXGISwapChainVTable::X]
    LOAD_FUNC(Present);
    LOAD_FUNC(ResizeBuffers);
    LOAD_FUNC(ResizeTarget);
#undef LOAD_FUNC
}

#endif//NO_OVERLAY