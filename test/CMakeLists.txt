file(GLOB TEST_FILES LIST_DIRECTORIES true "*.cpp")

message(STATUS "TEST_FILES: ${TEST_FILES}")

enable_testing()

file(GLOB_RECURSE SRC_FILES LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/src/*.cpp)
#remove main.cpp from the list
list(REMOVE_ITEM SRC_FILES ${CMAKE_SOURCE_DIR}/src/main.cpp)
message(STATUS "SRC_FILES: ${SRC_FILES}")

include(GoogleTest)
foreach(FILE ${TEST_FILES})
    get_filename_component(FILE_NAME ${FILE} NAME_WE)
    string(REPLACE "." "" FILE_NAME ${FILE_NAME})
    add_executable(
        ${FILE_NAME}
        ${SRC_FILES}
        ${FILE}
    )
    target_link_libraries(
        ${FILE_NAME}
        ${FREETYPE_LIBRARIES}
        fontconfig
        glfw
        glad
        stb
        lunasvg
        GTest::gtest_main
    )
    target_include_directories(${FILE_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/extern/glad/include
        ${CMAKE_SOURCE_DIR}/extern/stb
        ${CMAKE_SOURCE_DIR}/src
        PUBLIC ${FREETYPE_INCLUDE_DIRS}
    )
    gtest_discover_tests(${FILE_NAME})
endforeach()
