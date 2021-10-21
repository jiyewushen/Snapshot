cmake_minimum_required(VERSION 3.4.1)
#set(CMAKE_C_VISIBILITY_PRESET hidden)
#set(CMAKE_CXX_VISIBILITY_PRESET hidden)
add_subdirectory(yuv)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/metadata)
#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/metadata SOURCE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter -L${CMAKE_CURRENT_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -L${CMAKE_CURRENT_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}")
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/include)
add_library(ffmpeg SHARED ${SOURCE})
target_include_directories(ffmpeg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/include)
target_include_directories(ffmpeg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/yuv/)
#add_library(avcodec SHARED IMPORTED)
#set_target_properties(
#        avcodec
#        PROPERTIES IMPORTED_LOCATION
#       ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib/libavcodec.so)
#add_library(avformat SHARED IMPORTED)
#set_target_properties(
#        avformat
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib/libavformat.so)
#
#add_library(avutil SHARED IMPORTED)
#set_target_properties(
#        avutil
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib/libavutil.so)
#
#add_library(swscale SHARED IMPORTED)
#set_target_properties(
#        swscale
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib/libswscale.so)

#target_link_libraries(ffmpeg
#        log
#        jnigraphics
#        android
#        yuv
#        avformat
#        avcodec
#        swresample
#        avutil
#        )

target_link_libraries(ffmpeg
        log
        jnigraphics
        android
        yuv
        ffmpeg_core
        )