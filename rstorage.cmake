if (MSVC OR MSYS OR MINGW)
    add_definitions(-DRSTORAGE_FOR_WINDOWS)
endif ()

if (APPLE)
    add_definitions(-DRSTORAGE_FOR_APPLE)
endif ()

if (UNIX AND NOT APPLE)
    add_definitions(-DRSTORAGE_FOR_UNIX)
endif ()

set(RSTORAGE_DIRECTORIES  ${CMAKE_CURRENT_LIST_DIR}/ )
set(RSTORAGE_EXECUTABLE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/rstorage.c
        ${CMAKE_CURRENT_LIST_DIR}/storage/storage_fd.c
        ${CMAKE_CURRENT_LIST_DIR}/storage/storage_mcu.c)
