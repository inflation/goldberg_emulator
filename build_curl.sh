#! /bin/bash

# Build type (Debug or Release)
BUILD_TYPE="$1"
# where to build protobuf, must be win32 or win64
OUT_DIR="$2"

[ "$OUT_DIR" != "win32" -a "$OUT_DIR" != "win64" -a "$OUT_DIR" != "x86" -a "$OUT_DIR" != "x64" ] && echo "The output dir must be 'Win32', 'Win64', 'x86' or 'x64'" && exit 1
[ "$BUILD_TYPE" != "Debug" -a "$BUILD_TYPE" != "Release" ] && echo "The build type must be 'Debug' or 'Release'" && exit 1

# apt install libssl1.0-dev libssl1.0-dev:i386

# My variable to decide if we build x86 or x64 in CMakeLists.txt
if [ "$OUT_DIR" == "win32" -o "$OUT_DIR" == "x86" ]; then
  custom_arch_var="-DX86=ON"
else
  custom_arch_var="-DX64=ON"
fi

build_type="-DCMAKE_BUILD_TYPE=${BUILD_TYPE}"

build_http="-DHTTP_ONLY=ON"
build_exe="-DBUILD_CURL_EXE=OFF"
build_shared="-DBUILD_SHARED_LIBS=OFF"
build_testing="-DBUILD_TESTING=OFF"

args=()
args+=($build_http)
args+=($build_exe)
args+=($build_shared)
args+=($build_testing)
args+=($build_type)
args+=($custom_arch_var)

if [ ! -z "$EXTRA_CMAKE_ENV" ]; then
  args+=("-DCURL_STATIC_CRT=ON")
fi

# EXTRA_CMAKE_ENV is set by setup_clang_env.sh to build for windows.
# You must run setup_clang_env.sh before calling this script if you build for windows.

rm -rf "curl/$OUT_DIR" &&
mkdir "curl/$OUT_DIR" &&
cd "curl/$OUT_DIR" &&
echo "cmake -G \"Unix Makefiles\" $EXTRA_CMAKE_ENV \"${args[@]}\" .." &&
cmake -G "Unix Makefiles" $EXTRA_CMAKE_ENV "${args[@]}" .. &&
make -j${JOBS-2} || exit 1

exit 0
