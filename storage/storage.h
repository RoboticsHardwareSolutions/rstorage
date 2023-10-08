#pragma once

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"

#define MAX_SIZE_STORAGE_KBYTES 256
#define MAX_FILE_NAME_LENGHT 255

typedef enum
{
    storage_idle,
    storage_writing,
    storage_reading,
    storage_erasing,
    storage_error,
} storage_state_t;


#if defined(STM32F765xx) || defined(STM32G474xx) || defined(STM32F103xB)

struct storage_element
{
    storage_state_t         state;
    uint32_t                start_addr;
    uint32_t                size;
    struct storage_element* next;
};

#elif defined(RSTORAGE_USING_FD)

struct storage_element
{
    storage_state_t           state;
    char                    filename[MAX_FILE_NAME_LENGHT];
    int                     storage_index;
    int                     size;
    struct storage_element* next;
};

#endif
