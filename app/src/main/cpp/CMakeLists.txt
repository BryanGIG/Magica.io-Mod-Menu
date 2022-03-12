cmake_minimum_required(VERSION 3.18.1)
project("Magica.Mod")

find_library(log-lib log)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif ()

if (${CMAKE_ANDROID_ARCH_ABI} STREQUAL "armeabi-v7a")
    set(HOOK_FILES Substrate/hde64.c Substrate/SubstrateDebug.cpp Substrate/SubstrateHook.cpp Substrate/SubstratePosixMemory.cpp Substrate/SymbolFinder.cpp)
else()
    set(HOOK_FILES And64InlineHook/And64InlineHook.cpp)
endif()

include_directories(Includes)
add_library(polarmods SHARED
        Main.cpp
        ${HOOK_FILES}
        mainGame/Unity/API.cpp
        mainGame/Camera.cpp
        mainGame/GameCore.cpp
        mainGame/Character.cpp
        mainGame/PlayerData.cpp
        mainGame/PlayerBalance.cpp
        mainGame/BaseCharStatus.cpp)

target_link_libraries(polarmods ${log-lib})