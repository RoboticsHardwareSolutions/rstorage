#pragma once

#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"

#if defined(STM32F765xx) || defined(STM32G474xx) || defined(STM32F103xB)

#   include "rstorage_def.h"
#   include "rstorage.h"

struct storage_element
{
    rstorage_state          state;
    uint32_t                start_addr;
    uint32_t                size;
    struct storage_element* next;
};

typedef struct storage_element rstorage;

bool storage_init(rstorage* instance, int size_kbytes);

bool storage_write(rstorage* instance, void* data, uint32_t bytes);

bool storage_read(rstorage* instance, void* data, uint32_t bytes);

#elif defined(RSTORAGE_USING_FD)

#    include "rstorage_def.h"
#    include "rstorage.h"

struct storage_element
{
    rstorage_state          state;
    char                    filename[MAX_FILE_NAME_LENGHT];
    int                     storage_index;
    int                     size;
    struct storage_element* next;
};

typedef struct storage_element rstorage;

bool storage_init(rstorage* instance, int size_kbytes);

bool storage_write(rstorage* instance, void* data, uint32_t bytes);

bool storage_read(rstorage* instance, void* data, uint32_t bytes);

#endif  // defined(RSTORAGE_USING_FD)
