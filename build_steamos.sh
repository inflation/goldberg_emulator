rm -rf linux
mkdir -p linux/x86
mkdir -p linux/x86_64
../protobuf/prefix_x86/bin/protoc -I./dll/ --cpp_out=./dll/ ./dll/*.proto
g++ -m32 -shared -fPIC -fvisibility=hidden -Wl,--exclude-libs,ALL -DGNUC -DEMU_RELEASE_BUILD -DNDEBUG -s -o linux/x86/libsteam_api.so dll/*.cpp dll/*.cc -Wno-return-type -I../protobuf/prefix_x86/include/ -L../protobuf/prefix_x86/lib/ -lprotobuf-lite -std=c++11 && echo built32
../protobuf/prefix/bin/protoc -I./dll/ --cpp_out=./dll/ ./dll/*.proto
g++ -shared -fPIC -fvisibility=hidden -Wl,--exclude-libs,ALL -DGNUC -DEMU_RELEASE_BUILD -DNDEBUG -s -o linux/x86_64/libsteam_api.so dll/*.cpp dll/*.cc -Wno-return-type -I../protobuf/prefix/include/ -L../protobuf/prefix/lib/ -lprotobuf-lite -std=c++11 && echo built64
