cmake_minimum_required(VERSION 3.20)
project(tiny-webserver
    VERSION 0.0.1
    DESCRIPTION "使用C++实现一个简易的webserver"
    HOMEPAGE_URL "https://github.com/breeze-wink/tiny-Webserver"
    LANGUAGES CXX
)

# 设置C++标准为17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(plugin_output_Dir ${CMAKE_CURRENT_SOURCE_DIR}/plugin)

add_subdirectory(src)
add_subdirectory(test)
