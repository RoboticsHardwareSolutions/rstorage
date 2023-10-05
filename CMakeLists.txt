cmake_minimum_required(VERSION 3.9)
project(rstorage C)

file(GLOB RST_SOURCE1 storage/*.c)
file(GLOB RST_HEADERS1 storage/*.h)
file(GLOB RST_SOURCE2 *.c)
file(GLOB RST_HEADERS2 *.h)


add_library(${PROJECT_NAME} STATIC ${RST_SOURCE1} ${RST_HEADERS1} ${RST_SOURCE2} ${RST_HEADERS2})

add_executable(${PROJECT_NAME}_test tests/tests.c
        ${RST_SOURCE1} ${RST_HEADERS1}
        ${RST_SOURCE2} ${RST_HEADERS2}
        ./tests/runit/src/runit.c)


target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/storage>
        )

target_include_directories(${PROJECT_NAME}_test PUBLIC tests/runit/src storage .)


if (MSVC OR MSYS OR MINGW OR APPLE OR UNIX)
    target_compile_definitions(${PROJECT_NAME}_test PUBLIC -DRSTORAGE_USING_FD)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DRSTORAGE_USING_FD)
endif ()


set(FILES_FOR_FORMATTING tests/tests.c ${RST_SOURCE1} ${RST_HEADERS1} ${RST_SOURCE2} ${RST_HEADERS2})

# clang-format
find_program(clang_format NAMES clang-format)
if (NOT clang_format)
    message(STATUS "Could not locate clang-format")
else ()
    file(GLOB format_files ${FILES_FOR_FORMATTING})
    add_custom_target(rstorage_format COMMAND ${clang_format} -i -fallback-style=none -style=file --verbose ${format_files})
endif ()


