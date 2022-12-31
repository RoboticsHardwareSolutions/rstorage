if ((MSVC OR MSYS OR MINGW) OR (UNIX AND NOT APPLE) OR (APPLE))
    add_definitions(-DRSTORAGE_USING_FD)
endif ()

set(RSTORAGE_DIRECTORIES
        ${CMAKE_CURRENT_LIST_DIR}/
        ${CMAKE_CURRENT_LIST_DIR}/storage/)
set(RSTORAGE_EXECUTABLE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/rstorage.c
        ${CMAKE_CURRENT_LIST_DIR}/storage/storage_fd.c
        ${CMAKE_CURRENT_LIST_DIR}/storage/storage_mcu.c)
