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

# find fontconfig
find_package(Fontconfig REQUIRED)

if (FONTCONFIG_FOUND)
    message(STATUS "Fontconfig found")
else()
    message(FATAL_ERROR "Fontconfig not found")
endif()


include(FetchContent)

fetchContent_Declare(
    utf8cpp
    URL https://github.com/nemtrif/utfcpp/archive/refs/tags/v4.0.6.zip
)
FetchContent_MakeAvailable(utf8cpp)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#FetchContent_Declare(
    #lunasvg
    #GIT_REPOSITORY  https://github.com/sammycage/lunasvg.git
    #GIT_TAG         master 
#)
#FetchContent_MakeAvailable(lunasvg)


add_subdirectory(../lunasvg lunasvg)
add_subdirectory(extern/glad)
add_subdirectory(extern/stb)
add_subdirectory(src)
add_subdirectory(test_app)
# add_subdirectory(simple_slide_src)
add_subdirectory(test)
