cmake_minimum_required(VERSION 3.10.2)

project("Magica.Mod")

find_library(log-lib log)
find_package(dobby REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif ()

include_directories(Includes)

add_library(polarmods SHARED
        Main.cpp
        mainGame/Unity/API.cpp
        mainGame/Camera.cpp
        mainGame/GameCore.cpp
        mainGame/Character.cpp
        mainGame/Analytics.cpp)

target_link_libraries(polarmods ${log-lib} dobby::dobby)