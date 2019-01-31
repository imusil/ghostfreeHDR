cmake_minimum_required(VERSION 3.1)
project(hdr_lib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


find_package(OpenCV REQUIRED)

include_directories(inc)
include_directories(videoLib)


add_executable(hdr_lib main.cpp
        src/block_bilateral.cpp
        src/hdrlib_certainty_merge_uni.cpp
        src/hdrlib_certainty_merge_uni_bayer.cpp
        src/hdrlib_debayer.cpp
        src/hdrlib_core.cpp
        src/hdrlib_merge.cpp
        src/hdrlib_tonemap_durand.cpp)

target_link_libraries(hdr_lib ${OpenCV_LIBS})
