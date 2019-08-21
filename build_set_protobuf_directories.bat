SET PROTOBUF_X86_DIRECTORY=..\vcpkg\packages\protobuf_x86-windows-static
SET PROTOBUF_X64_DIRECTORY=..\vcpkg\packages\protobuf_x64-windows-static
rem location of protoc in protobuf directories:
SET PROTOC_DIRECTORY=\tools\protobuf\protoc.exe

if exist "%PROTOBUF_X64_DIRECTORY%\lib\libprotobuf-lite.lib" (
SET PROTOBUF_LIBRARY=\lib\libprotobuf-lite.lib
) else (
SET PROTOBUF_LIBRARY=\lib\libprotobuf.lib
)
