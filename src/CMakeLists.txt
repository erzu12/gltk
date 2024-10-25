file(GLOB_RECURSE SRC_DIR
    "*.h"
    "*.cpp"
)

add_library(${PROJECT_NAME}
    ${SRC_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)


target_link_libraries("${PROJECT_NAME}"
    glad
    glfw
    stb
    lunasvg
    fontconfig
    ${FREETYPE_LIBRARIES}
)

target_include_directories("${PROJECT_NAME}"
    PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include
    PUBLIC ${CMAKE_SOURCE_DIR}/../lunasvg/include
    PUBLIC ${CMAKE_SOURCE_DIR}/extern/stb
    PUBLIC ${FREETYPE_INCLUDE_DIRS}
)
