#pragma once

#if defined(RSTORAGE_USING_FD)

#    include "rstorage_def.h"
#    include "rstorage.h"
#    include "stdint.h"
#    include "stdbool.h"

struct storage_element
{
    rstorage_state          state;
    char                    filename[MAX_FILE_NAME_LENGHT];
    int                     storage_index;
    int                     size;
    struct storage_element* next;
};

typedef struct storage_element rstorage;

bool storage_fd_init(rstorage* instance, int size_kbytes);

bool storage_fd_write(rstorage* instance, void* data, uint32_t bytes);

bool storage_fd_read(rstorage* instance, void* data, uint32_t bytes);

#endif  // defined(RSTORAGE_USING_FD)
