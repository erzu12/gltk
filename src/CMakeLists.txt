file(GLOB_RECURSE SRC_DIR
    "*.h"
    "*.cpp"
)

# embed shader files from assets
file(GLOB_RECURSE SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../assets/shaders/*.*")

Message("Shader files:" ${SHADER_FILES})

set(HEADER_CONTENT "#pragma once\n\n")

foreach(shader ${SHADER_FILES})
    if(IS_DIRECTORY "${shader}")
        continue()
    endif()

    get_filename_component(name "${shader}" NAME)
    string(MAKE_C_IDENTIFIER "${name}" ident)

    file(READ "${shader}" content)

    string(APPEND HEADER_CONTENT "static const char ${ident}[] = R\"shader(${content})shader\";\n")
    string(APPEND HEADER_CONTENT "static const unsigned int ${ident}_len = sizeof(${ident}) - 1;\n\n")
endforeach()

file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/generated/embedded_shaders.h" "${HEADER_CONTENT}")

set(SRC_DIR ${SRC_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/generated/embedded_shaders.h")

add_library(${PROJECT_NAME}
    ${SRC_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries("${PROJECT_NAME}"
    glad
    stb
    lunasvg
    fontconfig
    utf8cpp
    -lglfw
    ${FREETYPE_LIBRARIES}
)


MESSAGE("gugus")
MESSAGE("${PROJECT_NAME}")
MESSAGE("${CMAKE_SOURCE_DIR}/src")

target_include_directories("${PROJECT_NAME}"
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../extern/glad/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../lunasvg/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../extern/stb
    PUBLIC ${FREETYPE_INCLUDE_DIRS}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${utf8cpp_SOURCE_DIR}/source
)
