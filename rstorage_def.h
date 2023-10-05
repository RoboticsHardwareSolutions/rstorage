#ifndef __RSTORAGE_DEF_H__
#define __RSTORAGE_DEF_H__

#include "stdint.h"

#define MAX_SIZE_STORAGE_KBYTES 256
#define MAX_FILE_NAME_LENGHT 255

typedef enum
{
    rstorage_idle,
    rstorage_writing,
    rstorage_reading,
    rstorage_erasing,
    rstorage_error,
} rstorage_state;

#if defined(RSTORAGE_USING_FD)
#    include "storage_fd.h"
#elif defined(STM32G474xx) || defined(STM32F103xB)
#    include "storage_mcu.h"
#endif


#endif
