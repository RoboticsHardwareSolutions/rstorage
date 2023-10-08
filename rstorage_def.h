#ifndef __RSTORAGE_DEF_H__
#define __RSTORAGE_DEF_H__


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

#include "storage.h"

#endif
