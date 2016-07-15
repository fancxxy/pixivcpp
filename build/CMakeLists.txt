cmake_minimum_required(VERSION 3.0)

project(pixivcpp)

set(SRC_LIST ../src/curl/CurlRequest.cpp
             ../src/pixiv/PixivCpp.cpp
             ../src/pixiv/CmdlineOption.cpp
             ../src/main.cpp)


set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++11 -Werror -Weverything -Wno-documentation -Wno-disabled-macro-expansion -Wno-float-equal -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-global-constructors -Wno-exit-time-destructors -Wno-missing-prototypes -Wno-padded -Wno-old-style-cast -Wno-weak-vtables")
set(CMAKE_EXE_LINKER_FLAGS "-lc++ -lc++abi -lcurl -ljsoncpp") 
set(CMAKE_BUILD_TYPE debug)

add_executable(pixivcpp ${SRC_LIST})
