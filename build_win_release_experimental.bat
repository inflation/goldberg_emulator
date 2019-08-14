mkdir release\experimental
del /Q release\experimental\*
call build_set_protobuf_directories.bat
%PROTOBUF_X86_DIRECTORY%%PROTOC_DIRECTORY% -I.\dll\ --cpp_out=.\dll\ .\dll\net.proto
call build_env_x86.bat
cl dll/rtlgenrandom.c dll/rtlgenrandom.def
cl /LD /DEMU_RELEASE_BUILD /DEMU_EXPERIMENTAL_BUILD /DNDEBUG /IImGui /Iglew\include /I%PROTOBUF_X86_DIRECTORY%\include\ dll/*.cpp dll/*.cc detours/*.cpp ImGui/*.cpp ImGui/impls/*.cpp overlay_experimental/*.cpp %PROTOBUF_X86_DIRECTORY%%PROTOBUF_LIBRARY% /LIBPATH:"glew\lib\Release\Win32\" Iphlpapi.lib Ws2_32.lib rtlgenrandom.lib Shell32.lib glew32s.lib /EHsc /MP12 /Ox /link /debug:none /OUT:release\experimental\steam_api.dll
cl /LD /DEMU_RELEASE_BUILD /DEMU_EXPERIMENTAL_BUILD /DNDEBUG steamclient.cpp /EHsc /MP12 /Ox /link /OUT:release\experimental\steamclient.dll
%PROTOBUF_X64_DIRECTORY%%PROTOC_DIRECTORY% -I.\dll\ --cpp_out=.\dll\ .\dll\net.proto
call build_env_x64.bat
cl dll/rtlgenrandom.c dll/rtlgenrandom.def
cl /LD /DEMU_RELEASE_BUILD /DEMU_EXPERIMENTAL_BUILD /DNDEBUG /IImGui /Iglew\include /I%PROTOBUF_X64_DIRECTORY%\include\ dll/*.cpp dll/*.cc detours/*.cpp ImGui/*.cpp ImGui/impls/*.cpp overlay_experimental/*.cpp %PROTOBUF_X64_DIRECTORY%%PROTOBUF_LIBRARY% /LIBPATH:"glew\lib\Release\x64\" Iphlpapi.lib Ws2_32.lib rtlgenrandom.lib Shell32.lib glew32s.lib /EHsc /MP12 /Ox /link /debug:none /OUT:release\experimental\steam_api64.dll
cl /LD /DEMU_RELEASE_BUILD /DEMU_EXPERIMENTAL_BUILD /DNDEBUG steamclient.cpp /EHsc /MP12 /Ox /link /OUT:release\experimental\steamclient64.dll
copy Readme_experimental.txt release\experimental\Readme.txt
