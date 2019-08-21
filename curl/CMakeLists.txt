#CMAKE_TOOLCHAIN_FILE

project(goldberg_emulator_protobuf)
cmake_minimum_required(VERSION 3.0)

if(WIN32)
  # Detect arch on Windows
  if( ${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(X64 ON)
  else()
    set(X86 ON)
  endif()

  if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
  else()
    add_compile_options(-std=c++11)
  endif()

  set(win_libs Iphlpapi ws2_32)
  if(X64)
    set(STEAM_NAME steam_api64)
  elseif(X86)
    set(STEAM_NAME steam_api)
  else()
    message(FATAL_ERROR "Arch unknown")
  endif()
elseif(APPLE)
  message(FATAL_ERROR "No CMake for Apple")
else()
  if(X64)
    set(CMAKE_C_FLAGS   "-m64")
    set(CMAKE_CXX_FLAGS "-m64")
  elseif(X86)
    set(CMAKE_C_FLAGS   "-m32")
    set(CMAKE_CXX_FLAGS "-m32")
  else()
    message(FATAL_ERROR "Arch unknown")
  endif()
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CURL_VERSION     "7.65.3")
set(CURL_DIR         ${CMAKE_CURRENT_SOURCE_DIR})
set(CURL_RELEASE_URL "https://curl.haxx.se/download/curl-${CURL_VERSION}.tar.xz")
set(CURL_SRC         curl-src)

if( NOT EXISTS ${CURL_DIR}/${CURL_SRC} )
  file(
    DOWNLOAD ${CURL_RELEASE_URL} ${CURL_DIR}/curl.tar.xz
    SHOW_PROGRESS
    EXPECTED_HASH MD5=7bd5b2ebfd3f591034eb8b55314d8c02
  )

  if( NOT EXISTS ${CURL_DIR}/curl.tar.xz )
    message(FATAL_ERROR "Download of curl failed")
  endif()

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar -xf curl.tar.xz
    WORKING_DIRECTORY ${CURL_DIR}
  )

  file(REMOVE ${CURL_DIR}/curl.tar.xz)
  file(RENAME ${CURL_DIR}/curl-${CURL_VERSION} "${CURL_SRC}")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(${CURL_DIR}/${CURL_SRC})
