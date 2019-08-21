set -e
rm -rf linux
mkdir -p linux/x86
mkdir -p linux/x86_64
mkdir -p linux/lobby_connect
mkdir -p linux/tools
cp scripts/find_interfaces.sh linux/tools/
../protobuf/prefix_x86/bin/protoc -I./dll/ --cpp_out=./dll/ ./dll/*.proto
g++ -m32 -shared -fPIC -fvisibility=hidden -Wl,--exclude-libs,ALL -DGNUC -DEMU_RELEASE_BUILD -DNDEBUG -s -o linux/x86/libsteam_api.so dll/*.cpp dll/*.cc -Wno-return-type -I../protobuf/prefix_x86/include/ -L../protobuf/prefix_x86/lib/ -lprotobuf-lite -std=c++11 && echo built32
g++ -m32 -fvisibility=hidden -Wl,--exclude-libs,ALL -DGNUC -DEMU_RELEASE_BUILD -DNDEBUG -DNO_DISK_WRITES -DLOBBY_CONNECT -s -o linux/lobby_connect/lobby_connect_x86 lobby_connect.cpp dll/*.cpp dll/*.cc -Wno-return-type -I../protobuf/prefix_x86/include/ -L../protobuf/prefix_x86/lib/ -lprotobuf-lite -lpthread -std=c++11 && echo built_lobby_connect32
../protobuf/prefix/bin/protoc -I./dll/ --cpp_out=./dll/ ./dll/*.proto
g++ -shared -fPIC -fvisibility=hidden -Wl,--exclude-libs,ALL -DGNUC -DEMU_RELEASE_BUILD -DNDEBUG -s -o linux/x86_64/libsteam_api.so dll/*.cpp dll/*.cc -Wno-return-type -I../protobuf/prefix/include/ -L../protobuf/prefix/lib/ -lprotobuf-lite -std=c++11 && echo built64
g++ -fvisibility=hidden -Wl,--exclude-libs,ALL -DGNUC -DEMU_RELEASE_BUILD -DNDEBUG -DNO_DISK_WRITES -DLOBBY_CONNECT -s -o linux/lobby_connect/lobby_connect_x64 lobby_connect.cpp dll/*.cpp dll/*.cc -Wno-return-type -I../protobuf/prefix/include/ -L../protobuf/prefix/lib/ -lprotobuf-lite -lpthread -std=c++11 && echo built_lobby_connect64
