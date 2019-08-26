#include "../dll/dll.h"
#include "Hook_Manager.h"

#ifndef NO_OVERLAY

#include "../detours/detours.h"

#ifdef STEAM_WIN32
#include "DX12_Hook.h"
#include "DX11_Hook.h"
#include "DX10_Hook.h"
#include "DX9_Hook.h"
#include "Windows_Hook.h"
#endif

#include "OpenGL_Hook.h"

#include <algorithm>

constexpr int max_hook_retries = 500;

#ifdef STEAM_WIN32
static decltype(&IDXGISwapChain::Present) _IDXGISwapChain_Present = nullptr;
static decltype(&IDirect3DDevice9::Present) _IDirect3DDevice9_Present = nullptr;
static decltype(&IDirect3DDevice9Ex::PresentEx) _IDirect3DDevice9Ex_PresentEx = nullptr;
static decltype(wglMakeCurrent)* _wglMakeCurrent = nullptr;

HRESULT STDMETHODCALLTYPE Hook_Manager::MyIDXGISwapChain_Present(IDXGISwapChain* _this, UINT SyncInterval, UINT Flags)
{
    Hook_Manager& inst = Hook_Manager::Inst();
    if (!inst.stop_retry())
    {
        IUnknown* pDevice = nullptr;
        _this->GetDevice(__uuidof(ID3D10Device), (void**)&pDevice);
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
                //_this->GetDevice(__uuidof(ID3D12Device), (void**)& pDevice);
                //if (pDevice)
                //{
                //    DX12_Hook* hook = DX12_Hook::Inst();
                //    if (hook->start_hook())
                //        inst.AddHook(hook);
                //}
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

void Hook_Manager::HookDXGIPresent(IDXGISwapChain *pSwapChain)
{
    if (!_dxgi_hooked)
    {
        _dxgi_hooked = true;
        (void*&)_IDXGISwapChain_Present = (*reinterpret_cast<void***>(pSwapChain))[(int)IDXGISwapChainVTable::Present];

        rendererdetect_hook->BeginHook();

        rendererdetect_hook->HookFuncs(
            std::pair<void**, void*>((PVOID*)& _IDXGISwapChain_Present, &Hook_Manager::MyIDXGISwapChain_Present)
        );

        rendererdetect_hook->EndHook();
    }
}

void Hook_Manager::HookDX9Present(IDirect3DDevice9* pDevice, bool ex)
{
    if (!_dx9_hooked)
    {
        _dx9_hooked = true;
        (void*&)_IDirect3DDevice9_Present = (*reinterpret_cast<void***>(pDevice))[(int)IDirect3DDevice9VTable::Present];
        if (ex)
            (void*&)_IDirect3DDevice9Ex_PresentEx = (*reinterpret_cast<void***>(pDevice))[(int)IDirect3DDevice9VTable::PresentEx];

        rendererdetect_hook->BeginHook();

        rendererdetect_hook->HookFuncs(
            std::pair<void**, void*>((PVOID*)& _IDirect3DDevice9_Present, &Hook_Manager::MyPresent)
        );
        if (ex)
        {
            rendererdetect_hook->HookFuncs(
                std::pair<void**, void*>((PVOID*)& _IDirect3DDevice9Ex_PresentEx, &Hook_Manager::MyPresentEx)
            );
        }

        rendererdetect_hook->EndHook();
    }
}

void Hook_Manager::HookwglMakeCurrent(BOOL(WINAPI* wglMakeCurrent)(HDC, HGLRC))
{
    if (!_ogl_hooked)
    {
        _ogl_hooked = true;

        _wglMakeCurrent = wglMakeCurrent;

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
        HWND hWnd = GetGameWindow();
        if (!hWnd)
            return;

        IDirect3D9Ex* pD3D = nullptr;
        IUnknown* pDevice = nullptr;
        HMODULE library = GetModuleHandle(DX9_Hook::DLL_NAME);
        decltype(Direct3DCreate9Ex)* Direct3DCreate9Ex = nullptr;
        if (library != nullptr)
        {
            Direct3DCreate9Ex = (decltype(Direct3DCreate9Ex))GetProcAddress(library, "Direct3DCreate9Ex");
            D3DPRESENT_PARAMETERS params = {};
            params.BackBufferWidth = 1;
            params.BackBufferHeight = 1;
            params.hDeviceWindow = hWnd;
            params.BackBufferCount = 1;
            params.Windowed = TRUE;
            params.SwapEffect = D3DSWAPEFFECT_DISCARD;

            if (Direct3DCreate9Ex != nullptr)
            {
                Direct3DCreate9Ex(D3D_SDK_VERSION, &pD3D);
                pD3D->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, NULL, reinterpret_cast<IDirect3DDevice9Ex * *>(&pDevice));
            }
            else
            {
                decltype(Direct3DCreate9)* Direct3DCreate9 = (decltype(Direct3DCreate9))GetProcAddress(library, "Direct3DCreate9");
                if (Direct3DCreate9 != nullptr)
                {
                    pD3D = reinterpret_cast<IDirect3D9Ex*>(Direct3DCreate9(D3D_SDK_VERSION));
                    pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, reinterpret_cast<IDirect3DDevice9 * *>(&pDevice));
                }
            }
        }
        
        if (pDevice != nullptr)
        {
            PRINT_DEBUG("Hooked D3D9::Present to detect DX Version\n");
            auto h = DX9_Hook::Inst();
            h->loadFunctions(reinterpret_cast<IDirect3DDevice9*>(pDevice), Direct3DCreate9Ex != nullptr);
            _hooks.insert(h);
            HookDX9Present(reinterpret_cast<IDirect3DDevice9*>(pDevice), Direct3DCreate9Ex != nullptr);
        }
        else
        {
            PRINT_DEBUG("Failed to hook D3D9::Present to detect DX Version\n");
        }

        if (pDevice) pDevice->Release();
        if (pD3D) pD3D->Release();
    }
}

void Hook_Manager::hook_dx10()
{
    if (!_dxgi_hooked && !_renderer_found)
    {
        HWND hWnd = GetGameWindow();
        if (!hWnd)
            return;

        IDXGISwapChain* pSwapChain = nullptr;
        ID3D10Device* pDevice = nullptr;
        HMODULE library = GetModuleHandle(DX10_Hook::DLL_NAME);
        if (library != nullptr)
        {
            decltype(D3D10CreateDeviceAndSwapChain)* D3D10CreateDeviceAndSwapChain =
                (decltype(D3D10CreateDeviceAndSwapChain))GetProcAddress(library, "D3D10CreateDeviceAndSwapChain");
            if (D3D10CreateDeviceAndSwapChain != nullptr)
            {
                DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};

                SwapChainDesc.BufferCount = 1;
                SwapChainDesc.BufferDesc.Width = 1;
                SwapChainDesc.BufferDesc.Height = 1;
                SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
                SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
                SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                SwapChainDesc.OutputWindow = hWnd;
                SwapChainDesc.SampleDesc.Count = 1;
                SwapChainDesc.SampleDesc.Quality = 0;
                SwapChainDesc.Windowed = TRUE;

                D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_NULL, NULL, 0, D3D10_SDK_VERSION, &SwapChainDesc, &pSwapChain, &pDevice);
            }
        }
        if (pSwapChain != nullptr)
        {
            PRINT_DEBUG("Hooked IDXGISwapChain::Present to detect DX Version\n");
            auto h = DX10_Hook::Inst();
            h->loadFunctions(pSwapChain);
            _hooks.insert(h);
            HookDXGIPresent(pSwapChain);
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
        HWND hWnd = GetGameWindow();
        if (!hWnd)
            return;

        IDXGISwapChain* pSwapChain = nullptr;
        ID3D11Device* pDevice = nullptr;
        HMODULE library = GetModuleHandle(DX11_Hook::DLL_NAME);
        if (library != nullptr)
        {
            decltype(D3D11CreateDeviceAndSwapChain)* D3D11CreateDeviceAndSwapChain =
                (decltype(D3D11CreateDeviceAndSwapChain))GetProcAddress(library, "D3D11CreateDeviceAndSwapChain");
            if (D3D11CreateDeviceAndSwapChain != nullptr)
            {
                DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};

                SwapChainDesc.BufferCount = 1;
                SwapChainDesc.BufferDesc.Width = 1;
                SwapChainDesc.BufferDesc.Height = 1;
                SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
                SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
                SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                SwapChainDesc.OutputWindow = hWnd;
                SwapChainDesc.SampleDesc.Count = 1;
                SwapChainDesc.SampleDesc.Quality = 0;
                SwapChainDesc.Windowed = TRUE;

                D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_NULL, NULL, 0, NULL, NULL, D3D11_SDK_VERSION, &SwapChainDesc, &pSwapChain, &pDevice, NULL, NULL);
            }
        }
        if (pSwapChain != nullptr)
        {
            PRINT_DEBUG("Hooked IDXGISwapChain::Present to detect DX Version\n");
            auto h = DX11_Hook::Inst();
            h->loadFunctions(pSwapChain);
            _hooks.insert(h);
            HookDXGIPresent(pSwapChain);
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
        HWND hWnd = GetGameWindow();
        if (!hWnd)
            return;

        IDXGIFactory4* pDXGIFactory = nullptr;
        IDXGISwapChain1* pSwapChain = nullptr;
        ID3D12CommandQueue* pCommandQueue = nullptr;
        ID3D12Device* pDevice = nullptr;
        ID3D12CommandAllocator* pCommandAllocator = nullptr;
        ID3D12CommandList* pCommandList = nullptr;

        HMODULE library = GetModuleHandle(DX12_Hook::DLL_NAME);
        if (library != nullptr)
        {
            decltype(D3D12CreateDevice)* D3D12CreateDevice =
                (decltype(D3D12CreateDevice))GetProcAddress(library, "D3D12CreateDevice");

            if (D3D12CreateDevice != nullptr)
            {
                D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pDevice));

                if (pDevice != nullptr)
                {
                    DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
                    SwapChainDesc.BufferCount = 2;
                    SwapChainDesc.Width = 1;
                    SwapChainDesc.Height = 1;
                    SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                    SwapChainDesc.Stereo = FALSE;
                    SwapChainDesc.SampleDesc = { 1, 0 };
                    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                    SwapChainDesc.Scaling = DXGI_SCALING_NONE;
                    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
                    SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

                    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
                    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
                    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
                    pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pCommandQueue));

                    if (pCommandQueue != nullptr)
                    {
                        HMODULE dxgi = GetModuleHandle("dxgi.dll");
                        if (dxgi != nullptr)
                        {
                            decltype(CreateDXGIFactory1)* CreateDXGIFactory1 = (decltype(CreateDXGIFactory1))GetProcAddress(dxgi, "CreateDXGIFactory1");
                            if (CreateDXGIFactory1 != nullptr)
                            {
                                CreateDXGIFactory1(IID_PPV_ARGS(&pDXGIFactory));
                                if (pDXGIFactory != nullptr)
                                {
                                    pDXGIFactory->CreateSwapChainForHwnd(pCommandQueue, hWnd, &SwapChainDesc, NULL, NULL, &pSwapChain);

                                    pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCommandAllocator));
                                    if (pCommandAllocator != nullptr)
                                    {
                                        pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCommandAllocator, NULL, IID_PPV_ARGS(&pCommandList));
                                    }
                                }
                            }
                        }
                    }
                }//if (pDevice != nullptr)
            }//if (D3D12CreateDevice != nullptr)
        }//if (library != nullptr)
        if (pSwapChain != nullptr && pCommandList != nullptr)
        {
            PRINT_DEBUG("Hooked IDXGISwapChain::Present to detect DX Version\n");

            auto h = DX12_Hook::Inst();
            h->loadFunctions(pCommandList, pSwapChain);
            _hooks.insert(h);
            HookDXGIPresent(pSwapChain);
        }
        else
        {
            PRINT_DEBUG("Failed to Hook IDXGISwapChain::Present to detect DX Version\n");
        }

        if (pCommandList) pCommandList->Release();
        if (pCommandAllocator) pCommandAllocator->Release();
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
        HMODULE library = GetModuleHandle(OpenGL_Hook::DLL_NAME);
        decltype(wglMakeCurrent)* wglMakeCurrent = nullptr;
        if (library != nullptr)
        {
            wglMakeCurrent = (decltype(wglMakeCurrent))GetProcAddress(library, "wglMakeCurrent");
        }
        if (wglMakeCurrent != nullptr)
        {
            PRINT_DEBUG("Hooked wglMakeCurrent to detect OpenGL\n");
            HookwglMakeCurrent(wglMakeCurrent);
        }
        else
        {
            PRINT_DEBUG("Failed to Hook wglMakeCurrent to detect OpenGL\n");
        }
    }
}

void Hook_Manager::create_hook(const char* libname)
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

//void Hook_Manager::create_hookW(const wchar_t *libname)
//{
//    if (!_wcsicmp(libname, L"d3d9.dll"))
//        Hook_Manager::Inst().hook_dx9();
//    else if (!_wcsicmp(libname, L"d3d10.dll"))
//        Hook_Manager::Inst().hook_dx10();
//    else if (!_wcsicmp(libname, L"d3d11.dll"))
//        Hook_Manager::Inst().hook_dx11();
//    else if (!_wcsicmp(libname, L"d3d12.dll"))
//        Hook_Manager::Inst().hook_dx12();
//    else if (!_wcsicmp(libname, L"opengl32.dll"))
//        Hook_Manager::Inst().hook_opengl();
//}
//
//HMODULE WINAPI Hook_Manager::MyLoadLibraryA(LPCTSTR lpLibFileName)
//{
//    auto res = _LoadLibraryA(lpLibFileName);
//    Hook_Manager::Inst().create_hookA(lpLibFileName);
//    return res;
//}
//
//HMODULE WINAPI Hook_Manager::MyLoadLibraryW(LPCWSTR lpLibFileName)
//{
//    auto res = _LoadLibraryW(lpLibFileName);
//    Hook_Manager::Inst().create_hookW(lpLibFileName);
//    return res;
//}
//
//HMODULE WINAPI Hook_Manager::MyLoadLibraryExA(LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
//{
//    auto res = _LoadLibraryA(lpLibFileName);
//    Hook_Manager::Inst().create_hookA(lpLibFileName);
//    return res;
//}
//
//HMODULE WINAPI Hook_Manager::MyLoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
//{
//    auto res = _LoadLibraryExW(lpLibFileName, hFile, dwFlags);
//    Hook_Manager::Inst().create_hookW(lpLibFileName);
//    return res;
//}

bool Hook_Manager::stop_retry()
{
    // Retry or not
    bool stop = ++_hook_retries >= max_hook_retries;

    if (stop)
        FoundRenderer(nullptr);

    return stop;
}

void Hook_Manager::find_renderer(Hook_Manager* _this)
{
    _this->rendererdetect_hook = new Base_Hook();
    _this->AddHook(_this->rendererdetect_hook);

    std::vector<std::string> const libraries = { "opengl32.dll", "d3d12.dll", "d3d11.dll", "d3d10.dll", "d3d9.dll" };

    while (!_this->_renderer_found && !_this->stop_retry())
    {
        std::vector<std::string>::const_iterator it = libraries.begin();
        while (it != libraries.end())
        {
            it = std::find_if(it, libraries.end(), [](std::string const& name) {
                auto x = GetModuleHandle(name.c_str());
                if (x != NULL)
                    return true;
                return false;
            });

            if (it == libraries.end())
                break;

            _this->create_hook(it->c_str());
            ++it;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
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
    _hook_thread = new std::thread(&Hook_Manager::find_renderer, this);
#endif
}

void Hook_Manager::FoundRenderer(Base_Hook* hook)
{
    if (!_renderer_found)
    {
        _renderer_found = true;
        game_renderer = hook;

        if (hook == nullptr)
            PRINT_DEBUG("We found a renderer but couldn't hook it, aborting overlay hook.\n");
        else
            PRINT_DEBUG("Hooked renderer in %d tries\n", _hook_retries);

        _hook_thread->join();
        delete _hook_thread;
        _hook_thread = nullptr;

        // Remove all hooks that are unused
        std::set<Base_Hook*>::iterator item;
        while ((item = std::find_if(_hooks.begin(), _hooks.end(), [hook](Base_Hook* item) {return item != hook; })) != _hooks.end())
        {
            delete *item;
            _hooks.erase(item);
        }
    }
}

#endif//NO_OVERLAY