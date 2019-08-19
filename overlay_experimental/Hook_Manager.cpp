#include "../dll/dll.h"
#include "Hook_Manager.h"

#ifndef NO_OVERLAY

#include "../detours/detours.h"

#ifdef STEAM_WIN32
#include "DX12_Hook.h"
#include "DX11_Hook.h"
#include "DX10_Hook.h"
#include "DX9_Hook.h"
#endif

#include "OpenGL_Hook.h"

#include <algorithm>

#ifdef STEAM_WIN32
decltype(LoadLibraryA    )* _LoadLibraryA     = LoadLibraryA;
decltype(LoadLibraryW    )* _LoadLibraryW     = LoadLibraryW;
decltype(LoadLibraryExA  )* _LoadLibraryExA   = LoadLibraryExA;
decltype(LoadLibraryExW  )* _LoadLibraryExW   = LoadLibraryExW;

decltype(&IDXGISwapChain::Present) _IDXGISwapChain_Present;
decltype(&IDirect3DDevice9::Present) _IDirect3DDevice9_Present;
decltype(&IDirect3DDevice9Ex::PresentEx) _IDirect3DDevice9Ex_PresentEx;
decltype(wglMakeCurrent)* _wglMakeCurrent;

HRESULT STDMETHODCALLTYPE Hook_Manager::MyIDXGISwapChain_Present(IDXGISwapChain* _this, UINT SyncInterval, UINT Flags)
{
    Hook_Manager& inst = Hook_Manager::Inst();
    if (!inst.stop_retry())
    {
        IUnknown* pDevice;
        _this->GetDevice(__uuidof(ID3D10Device), (void**)& pDevice);
        if (pDevice)
        {
            DX10_Hook* hook = DX10_Hook::Inst();
            if (hook->start_hook())
                inst.AddHook(hook);
        }
        else
        {
            _this->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice);
            if (pDevice)
            {
                DX11_Hook* hook = DX11_Hook::Inst();
                if (hook->start_hook())
                    inst.AddHook(hook);
            }
            else
            {
                _this->GetDevice(__uuidof(ID3D12Device), (void**)& pDevice);
                if (pDevice)
                {
                //    DX12_Hook* hook = DX12_Hook::Inst();
                //    if (hook->start_hook())
                //        inst.AddHook(hook);
                }
            }
        }
        if (pDevice) pDevice->Release();
    }

    return (_this->*_IDXGISwapChain_Present)(SyncInterval, Flags);
}

HRESULT STDMETHODCALLTYPE Hook_Manager::MyPresent(IDirect3DDevice9* _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
    Hook_Manager& inst = Hook_Manager::Inst();
    if (!inst.stop_retry())
    {
        DX9_Hook* hook = DX9_Hook::Inst();
        if (hook->start_hook())
            inst.AddHook(hook);
    }
    return (_this->*_IDirect3DDevice9_Present)(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT STDMETHODCALLTYPE Hook_Manager::MyPresentEx(IDirect3DDevice9Ex* _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
{
    Hook_Manager& inst = Hook_Manager::Inst();
    if (!inst.stop_retry())
    {
        DX9_Hook* hook = DX9_Hook::Inst();
        if (hook->start_hook())
            inst.AddHook(hook);
    }
    return (_this->*_IDirect3DDevice9Ex_PresentEx)(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

BOOL WINAPI Hook_Manager::MywglMakeCurrent(HDC hDC, HGLRC hGLRC)
{
    Hook_Manager& inst = Hook_Manager::Inst();
    if (!inst.stop_retry())
    {
        OpenGL_Hook* hook = OpenGL_Hook::Inst();
        if (hook->start_hook())
            inst.AddHook(hook);
    }
    return _wglMakeCurrent(hDC, hGLRC);
}

void Hook_Manager::HookDXGIPresent()
{
    if (!_dxgi_hooked)
    {
        _dxgi_hooked = true;
        rendererdetect_hook->BeginHook();

        rendererdetect_hook->HookFuncs(
            std::pair<void**, void*>((PVOID*)& _IDXGISwapChain_Present, &Hook_Manager::MyIDXGISwapChain_Present)
        );

        rendererdetect_hook->EndHook();
    }
}

void Hook_Manager::HookDX9Present()
{
    if (!_dx9_hooked)
    {
        _dx9_hooked = true;
        rendererdetect_hook->BeginHook();

        rendererdetect_hook->HookFuncs(
            std::pair<void**, void*>((PVOID*)& _IDirect3DDevice9_Present, &Hook_Manager::MyPresent),
            std::pair<void**, void*>((PVOID*)& _IDirect3DDevice9Ex_PresentEx, &Hook_Manager::MyPresentEx)
        );

        rendererdetect_hook->EndHook();
    }
}

void Hook_Manager::HookwglMakeCurrent()
{
    if (!_ogl_hooked)
    {
        _ogl_hooked = true;
        rendererdetect_hook->BeginHook();

        rendererdetect_hook->HookFuncs(
            std::pair<void**, void*>((PVOID*)& _wglMakeCurrent, &Hook_Manager::MywglMakeCurrent)
        );

        rendererdetect_hook->EndHook();
    }
}

void Hook_Manager::hook_dx9()
{
    if (!_dx9_hooked && !_renderer_found)
    {
        IDirect3D9Ex* pD3D;
        IDirect3DDevice9Ex* pDeviceEx;
        decltype(Direct3DCreate9Ex)* Direct3DCreate9Ex = (decltype(Direct3DCreate9Ex))GetProcAddress(GetModuleHandle(DX9_Hook::DLL_NAME), "Direct3DCreate9Ex");

        Direct3DCreate9Ex(D3D_SDK_VERSION, &pD3D);

        D3DPRESENT_PARAMETERS params = {};
        params.BackBufferWidth = 1;
        params.BackBufferHeight = 1;
        params.hDeviceWindow = GetForegroundWindow();

        pD3D->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, NULL, &pDeviceEx);

        if (pDeviceEx)
        {
            PRINT_DEBUG("Hooked D3D9::Present to detect DX Version\n");
            (void*&)_IDirect3DDevice9_Present = (*reinterpret_cast<void***>(pDeviceEx))[(int)IDirect3DDevice9VTable::Present];
            (void*&)_IDirect3DDevice9Ex_PresentEx = (*reinterpret_cast<void***>(pDeviceEx))[(int)IDirect3DDevice9VTable::PresentEx];
            HookDX9Present();
        }
        else
        {
            PRINT_DEBUG("Failed to hook D3D9::Present to detect DX Version\n");
        }

        if (pDeviceEx)pDeviceEx->Release();
        if (pD3D)pD3D->Release();
    }
}

void Hook_Manager::hook_dx10()
{
    if (!_dxgi_hooked && !_renderer_found)
    {
        IDXGISwapChain* pSwapChain;
        ID3D10Device* pDevice;
        DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
        decltype(D3D10CreateDeviceAndSwapChain)* D3D10CreateDeviceAndSwapChain =
            (decltype(D3D10CreateDeviceAndSwapChain))GetProcAddress(GetModuleHandle(DX10_Hook::DLL_NAME), "D3D10CreateDeviceAndSwapChain");
        SwapChainDesc.BufferCount = 1;
        SwapChainDesc.BufferDesc.Width = 1;
        SwapChainDesc.BufferDesc.Height = 1;
        SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
        SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        SwapChainDesc.OutputWindow = GetForegroundWindow();
        SwapChainDesc.SampleDesc.Count = 1;
        SwapChainDesc.SampleDesc.Quality = 0;
        SwapChainDesc.Windowed = TRUE;

        D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &SwapChainDesc, &pSwapChain, &pDevice);

        if (pDevice != nullptr && pSwapChain != nullptr)
        {
            PRINT_DEBUG("Hooked IDXGISwapChain::Present to detect DX Version\n");
            (void*&)_IDXGISwapChain_Present = (*reinterpret_cast<void***>(pSwapChain))[(int)IDXGISwapChainVTable::Present];
            HookDXGIPresent();
        }
        else
        {
            PRINT_DEBUG("Failed to Hook IDXGISwapChain::Present to detect DX Version\n");
        }
        if (pDevice)pDevice->Release();
        if (pSwapChain)pSwapChain->Release();
    }
}

void Hook_Manager::hook_dx11()
{
    if (!_dxgi_hooked && !_renderer_found)
    {
        IDXGISwapChain* pSwapChain;
        ID3D11Device* pDevice;
        DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
        decltype(D3D11CreateDeviceAndSwapChain)* D3D11CreateDeviceAndSwapChain =
            (decltype(D3D11CreateDeviceAndSwapChain))GetProcAddress(GetModuleHandle(DX11_Hook::DLL_NAME), "D3D11CreateDeviceAndSwapChain");
        SwapChainDesc.BufferCount = 1;
        SwapChainDesc.BufferDesc.Width = 1;
        SwapChainDesc.BufferDesc.Height = 1;
        SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
        SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        SwapChainDesc.OutputWindow = GetForegroundWindow();
        SwapChainDesc.SampleDesc.Count = 1;
        SwapChainDesc.SampleDesc.Quality = 0;
        SwapChainDesc.Windowed = TRUE;

        D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, NULL, D3D11_SDK_VERSION, &SwapChainDesc, &pSwapChain, &pDevice, NULL, NULL);

        if (pDevice != nullptr && pSwapChain != nullptr)
        {
            PRINT_DEBUG("Hooked IDXGISwapChain::Present to detect DX Version\n");
            (void*&)_IDXGISwapChain_Present = (*reinterpret_cast<void***>(pSwapChain))[(int)IDXGISwapChainVTable::Present];
            HookDXGIPresent();
        }
        else
        {
            PRINT_DEBUG("Failed to Hook IDXGISwapChain::Present to detect DX Version\n");
        }

        if (pDevice) pDevice->Release();
        if (pSwapChain) pSwapChain->Release();
    }
}

void Hook_Manager::hook_dx12()
{
    if (!_dxgi_hooked && !_renderer_found)
    {
        IDXGIFactory4* pDXGIFactory = nullptr;
        IDXGISwapChain1* pSwapChain = nullptr;
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        ID3D12CommandQueue* pCommandQueue = nullptr;
        ID3D12Device* pDevice = nullptr;
        decltype(D3D12CreateDevice)* D3D12CreateDevice =
            (decltype(D3D12CreateDevice))GetProcAddress(GetModuleHandle(DX12_Hook::DLL_NAME), "D3D12CreateDevice");

        D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pDevice));

        if (pDevice)
        {
            DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
            SwapChainDesc.Width = 0;
            SwapChainDesc.Height = 0;
            SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            SwapChainDesc.Stereo = FALSE;
            SwapChainDesc.SampleDesc = { 1, 0 };
            SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            SwapChainDesc.BufferCount = 3;
            SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
            SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
            pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pCommandQueue));

            if (pCommandQueue)
            {
                reinterpret_cast<decltype(CreateDXGIFactory1)*>(GetProcAddress(GetModuleHandle("dxgi.dll"), "CreateDXGIFactory1"))(IID_PPV_ARGS(&pDXGIFactory));
                pDXGIFactory->CreateSwapChainForHwnd(pCommandQueue, GetForegroundWindow(), &SwapChainDesc, NULL, NULL, &pSwapChain);
                if (pSwapChain != nullptr)
                {
                    PRINT_DEBUG("Hooked IDXGISwapChain::Present to detect DX Version\n");
                    (void*&)_IDXGISwapChain_Present = (*reinterpret_cast<void***>(pSwapChain))[(int)IDXGISwapChainVTable::Present];
                    HookDXGIPresent();
                }
                else
                {
                    PRINT_DEBUG("Failed to Hook IDXGISwapChain::Present to detect DX Version\n");
                }
            }
        }

        if (pSwapChain) pSwapChain->Release();
        if (pDXGIFactory) pDXGIFactory->Release();
        if (pCommandQueue) pCommandQueue->Release();
        if (pDevice) pDevice->Release();
    }
}

void Hook_Manager::hook_opengl()
{
    if (!_ogl_hooked && !_renderer_found)
    {
        _wglMakeCurrent = (decltype(_wglMakeCurrent))GetProcAddress(GetModuleHandle(OpenGL_Hook::DLL_NAME), "wglMakeCurrent");
        HookwglMakeCurrent();
    }
}

void Hook_Manager::create_hookA(const char* libname)
{
    if (!_stricmp(libname, "d3d9.dll"))
        Hook_Manager::Inst().hook_dx9();
    else if (!_stricmp(libname, "d3d10.dll"))
        Hook_Manager::Inst().hook_dx10();
    else if (!_stricmp(libname, "d3d11.dll"))
        Hook_Manager::Inst().hook_dx11();
    else if (!_stricmp(libname, "d3d12.dll"))
        Hook_Manager::Inst().hook_dx12();
    else if (!_stricmp(libname, "opengl32.dll"))
        Hook_Manager::Inst().hook_opengl();
}

void Hook_Manager::create_hookW(const wchar_t *libname)
{
    if (!_wcsicmp(libname, L"d3d9.dll"))
        Hook_Manager::Inst().hook_dx9();
    else if (!_wcsicmp(libname, L"d3d10.dll"))
        Hook_Manager::Inst().hook_dx10();
    else if (!_wcsicmp(libname, L"d3d11.dll"))
        Hook_Manager::Inst().hook_dx11();
    else if (!_wcsicmp(libname, L"d3d12.dll"))
        Hook_Manager::Inst().hook_dx12();
    else if (!_wcsicmp(libname, L"opengl32.dll"))
        Hook_Manager::Inst().hook_opengl();
}

HMODULE WINAPI Hook_Manager::MyLoadLibraryA(LPCTSTR lpLibFileName)
{
    auto res = _LoadLibraryA(lpLibFileName);
    Hook_Manager::Inst().create_hookA(lpLibFileName);
    return res;
}

HMODULE WINAPI Hook_Manager::MyLoadLibraryW(LPCWSTR lpLibFileName)
{
    auto res = _LoadLibraryW(lpLibFileName);
    Hook_Manager::Inst().create_hookW(lpLibFileName);
    return res;
}

HMODULE WINAPI Hook_Manager::MyLoadLibraryExA(LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
{
    auto res = _LoadLibraryA(lpLibFileName);
    Hook_Manager::Inst().create_hookA(lpLibFileName);
    return res;
}

HMODULE WINAPI Hook_Manager::MyLoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
{
    auto res = _LoadLibraryExW(lpLibFileName, hFile, dwFlags);
    Hook_Manager::Inst().create_hookW(lpLibFileName);
    return res;
}

bool Hook_Manager::stop_retry()
{
    // Retry 200 times, we look for rendering functions so its actually: "retry for 200 frames"
    bool stop = ++_hook_retries >= 200;

    if (stop)
    {
        PRINT_DEBUG("We found a renderer but couldn't hook it, aborting overlay hook.\n");
        FoundRenderer(nullptr);
    }

    return stop;
}

void Hook_Manager::HookLoadLibrary()
{
    if (!_renderer_found && !_loadlibrary_hooked)
    {
        _loadlibrary_hooked = true;

        rendererdetect_hook = new Base_Hook();
        AddHook(rendererdetect_hook);

        rendererdetect_hook->BeginHook();

        rendererdetect_hook->HookFuncs(
            std::pair<void**, void*>((PVOID*)& _LoadLibraryA, &Hook_Manager::MyLoadLibraryA),
            std::pair<void**, void*>((PVOID*)& _LoadLibraryW, &Hook_Manager::MyLoadLibraryW),
            std::pair<void**, void*>((PVOID*)& _LoadLibraryExA, &Hook_Manager::MyLoadLibraryExA),
            std::pair<void**, void*>((PVOID*)& _LoadLibraryExW, &Hook_Manager::MyLoadLibraryExW)
        );

        rendererdetect_hook->EndHook();
    }
}

#endif

Hook_Manager::Hook_Manager():
    _hook_retries(0),
#ifdef STEAM_WIN32
    _loadlibrary_hooked(false),
    _dxgi_hooked(false),
    _dx9_hooked(false),
#endif
    _renderer_found(false),
    _ogl_hooked(false)
{}

Hook_Manager::~Hook_Manager()
{
    for (auto& i : _hooks)
        delete i;
}

Hook_Manager& Hook_Manager::Inst()
{
    static Hook_Manager hook;
    return hook;
}

void Hook_Manager::HookRenderer()
{
#ifdef STEAM_WIN32
    HookLoadLibrary();
    std::vector<std::string> const libraries = { "opengl32.dll", "d3d12.dll", "d3d11.dll", "d3d10.dll", "d3d9.dll" };
    std::vector<std::string>::const_iterator it = libraries.begin();
    while (it != libraries.end())
    {
        it = std::find_if(it, libraries.end(), [](std::string const& name) {
            auto x = GetModuleHandle(name.c_str());
            if (x != 0 && x != INVALID_HANDLE_VALUE)
                return true;
            return false;
            });

        if (it == libraries.end())
            break;

        create_hookA(it->c_str());
        ++it;
    }
#endif
}

void Hook_Manager::FoundRenderer(Base_Hook* hook)
{
    if (!_renderer_found)
    {
        _renderer_found = true;

        // Remove all hooks that are unused
        _hooks.erase(std::remove_if(_hooks.begin(), _hooks.end(), [&hook](Base_Hook* it_hook) {
            if (hook != it_hook)
            {
                delete it_hook;
                return true;
            }
            return false;
        }), _hooks.end());

        _loadlibrary_hooked = false;
    }
}

#endif//NO_OVERLAY