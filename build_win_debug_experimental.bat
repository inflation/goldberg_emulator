@echo off
cd /d "%~dp0"
call build_set_protobuf_directories.bat
"%PROTOC_X86_EXE%" -I.\dll\ --cpp_out=.\dll\ .\dll\net.proto
call build_env_x86.bat
cl dll/rtlgenrandom.c dll/rtlgenrandom.def
cl /LD /I%PROTOBUF_X86_DIRECTORY%\include\ /DEMU_EXPERIMENTAL_BUILD dll/*.cpp dll/*.cc detours/*.cpp "%PROTOBUF_X86_LIBRARY%" Iphlpapi.lib Ws2_32.lib rtlgenrandom.lib Shell32.lib /EHsc /MP12 /link /OUT:steam_api.dll
cl /LD steamclient.cpp /EHsc /MP12 /link /OUT:steamclient.dll
cl /LD steamnetworkingsockets.cpp /EHsc /MP12 /link /OUT:steamnetworkingsockets.dll

"%PROTOC_X64_EXE%" -I.\dll\ --cpp_out=.\dll\ .\dll\net.proto
call build_env_x64.bat
cl dll/rtlgenrandom.c dll/rtlgenrandom.def
cl /LD /I%PROTOBUF_X64_DIRECTORY%\include\ /DEMU_EXPERIMENTAL_BUILD dll/*.cpp dll/*.cc detours/*.cpp "%PROTOBUF_X64_LIBRARY%" Iphlpapi.lib Ws2_32.lib rtlgenrandom.lib Shell32.lib /EHsc /MP12 /link /OUT:steam_api64.dll
cl /LD steamclient.cpp /EHsc /MP12 /link /OUT:steamclient64.dll
cl /LD steamnetworkingsockets.cpp /EHsc /MP12 /link /OUT:steamnetworkingsockets64.dll
