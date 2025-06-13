cmake_minimum_required(VERSION 3.24)
project(rstorage
        VERSION 0.1.0
        LANGUAGES C
        DESCRIPTION
        "rstorage simple library for make binary storage in memory")

file(GLOB RST_SOURCE1 storage/*.c)
file(GLOB RST_HEADERS1 storage/*.h)
file(GLOB RST_SOURCE2 *.c)
file(GLOB RST_HEADERS2 *.h)


add_library(${PROJECT_NAME} STATIC ${RST_SOURCE1} ${RST_HEADERS1} ${RST_SOURCE2} ${RST_HEADERS2})
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/storage>
)

#################### TEST SECTION #######################
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES Generic)
    add_executable(${PROJECT_NAME}_test test/tests.c)

    if (NOT TARGET runit)
        include(FetchContent)
        FetchContent_Declare(
                runit
                GIT_REPOSITORY https://github.com/RoboticsHardwareSolutions/runit.git
                GIT_TAG main
        )
        FetchContent_MakeAvailable(runit)
    endif ()

    target_link_libraries(${PROJECT_NAME}_test PRIVATE rstorage runit)
    if (MSVC OR MSYS OR MINGW OR APPLE OR UNIX)
        target_compile_definitions(${PROJECT_NAME}_test PUBLIC -DRSTORAGE_USING_FD)
        target_compile_definitions(${PROJECT_NAME} PUBLIC -DRSTORAGE_USING_FD)
    endif ()

    enable_testing()
    add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)
endif ()


#################### FORMAT SECTION #######################
include(FetchContent)
FetchContent_Declare(
        rlibhelper
        GIT_REPOSITORY https://github.com/RoboticsHardwareSolutions/rlibhelper.git
        GIT_TAG main
)
FetchContent_MakeAvailable(rlibhelper)
set(FILES_FOR_FORMATTING test/tests.c ${RST_SOURCE1} ${RST_HEADERS1} ${RST_SOURCE2} ${RST_HEADERS2})
include(${CMAKE_BINARY_DIR}/_deps/rlibhelper-src/format.cmake)
format_files(${PROJECT_NAME} SOURCES ${FILES_FOR_FORMATTING})



