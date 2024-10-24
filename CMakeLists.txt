cmake_minimum_required(VERSION 3.16)
project(save_state)

option(SAVE_STATE_BUILD_TESTS "Build tests for save state" false)
option(SAVE_STATE_BUILD_SHARED "Build save state as a shared library" false)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (!WIN32)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wold-style-cast -Wcast-align -Wunused -Wconversion -Wuseless-cast -Wnull-dereference -Wlogical-op $<$<CONFIG:DEBUG>:-fsanitize=address,undefined,leak>")
else ()
    set(CMAKE_CXX_FLAGS "")
endif ()

if (SAVE_STATE_BUILD_SHARED)
    add_library(save_state SHARED src/save_state.hpp src/save_state.cpp)
else()
    add_library(save_state src/save_state.hpp src/save_state.cpp)
endif()

if (SAVE_STATE_BUILD_TESTS) 
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

# For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    enable_testing()

    include(GoogleTest)

    add_subdirectory(tests)
endif()
