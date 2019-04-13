del /Q /S release\*
rmdir /S /Q release\experimental
rmdir /S /Q release\lobby_connect
rmdir /S /Q release
mkdir release
call build_set_protobuf_directories.bat
%PROTOBUF_X86_DIRECTORY%%PROTOC_DIRECTORY% -I.\dll\ --cpp_out=.\dll\ .\dll\net.proto
call build_env_x86.bat
cl dll/rtlgenrandom.c dll/rtlgenrandom.def
cl /LD /DEMU_RELEASE_BUILD /DNDEBUG /I%PROTOBUF_X86_DIRECTORY%\include\ dll/*.cpp dll/*.cc %PROTOBUF_X86_DIRECTORY%%PROTOBUF_LIBRARY% Iphlpapi.lib Ws2_32.lib rtlgenrandom.lib Shell32.lib /EHsc /MP12 /Ox /link /debug:none /OUT:release\steam_api.dll
%PROTOBUF_X64_DIRECTORY%%PROTOC_DIRECTORY% -I.\dll\ --cpp_out=.\dll\ .\dll\net.proto
call build_env_x64.bat
cl dll/rtlgenrandom.c dll/rtlgenrandom.def
cl /LD /DEMU_RELEASE_BUILD /DNDEBUG /I%PROTOBUF_X64_DIRECTORY%\include\ dll/*.cpp dll/*.cc %PROTOBUF_X64_DIRECTORY%%PROTOBUF_LIBRARY% Iphlpapi.lib Ws2_32.lib rtlgenrandom.lib Shell32.lib /EHsc /MP12 /Ox /link /debug:none /OUT:release\steam_api64.dll
copy Readme_release.txt release\Readme.txt
xcopy /s files_example\* release\
call build_win_release_experimental.bat
call build_win_lobby_connect.bat
