cmake_minimum_required(VERSION 3.16.2)
project(gltk)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# finde freetype
find_package(Freetype REQUIRED)

if (FREETYPE_FOUND)
    message(STATUS "Freetype found")
else()
    message(FATAL_ERROR "Freetype not found")
endif()


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_subdirectory(extern/glad)
add_subdirectory(src)
# add_subdirectory(test_app)
add_subdirectory(simple_slide_src)
add_subdirectory(test)
