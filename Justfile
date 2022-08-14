build_proto:
    #!/bin/bash
    set -ex
    mkdir build && cd build
    curl -LO https://github.com/protocolbuffers/protobuf/releases/download/v21.5/protobuf-cpp-3.21.5.tar.gz
    tar xf protobuf-cpp-3.21.5.tar.gz
    cd protobuf-3.21.5
    cmake . -GNinja \
            -DCMAKE_INSTALL_PREFIX=../protobuf \
            -Dprotobuf_BUILD_TESTS=OFF \
            -DBUILD_SHARED_LIBS=OFF \
            -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
    ninja install
build: build_proto
    #!/bin/bash
    cd build
    cmake .. -GNinja
    ninja