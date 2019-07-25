#ifndef __IMGUI_SHADER_BLOBS_INCLUDED__
#define __IMGUI_SHADER_BLOBS_INCLUDED__

// Defining this will use d3dcompiler and it will be a dependence of the dll.
//#define USE_D3DCOMPILE

#ifdef USE_D3DCOMPILE
    #include <d3dcompiler.h>
  #ifdef _MSC_VER
    #pragma comment(lib, "d3dcompiler") // Automatically link with d3dcompiler.lib as we are using D3DCompile() below.
  #endif
#else

#define ImGui_vertexShaderDX10_len 876
extern unsigned char ImGui_vertexShaderDX10[ImGui_vertexShaderDX10_len];

#define ImGui_pixelShaderDX10_len 660
extern unsigned char ImGui_pixelShaderDX10[ImGui_pixelShaderDX10_len];

#define ImGui_vertexShaderDX11_len ImGui_vertexShaderDX10_len
#define ImGui_vertexShaderDX11     ImGui_vertexShaderDX10

#define ImGui_pixelShaderDX11_len ImGui_pixelShaderDX10_len
#define ImGui_pixelShaderDX11     ImGui_pixelShaderDX10

#define ImGui_vertexShaderDX12_len 988
extern unsigned char ImGui_vertexShaderDX12[ImGui_vertexShaderDX12_len];

#define ImGui_pixelShaderDX12_len 736
extern unsigned char ImGui_pixelShaderDX12[ImGui_pixelShaderDX12_len];

#endif

#endif//__IMGUI_SHADER_BLOBS_INCLUDED__