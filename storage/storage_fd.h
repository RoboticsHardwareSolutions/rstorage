#ifndef __STORAGE_FD_H_
#define __STORAGE_FD_H_

#include "rstorage_def.h"
#include "stdint.h"
#include "stdbool.h"

struct storage_element
{
    rstorage_state   state;
    uint32_t         size;
    bool             data_recorded;
    uint8_t          checksum;
    struct storage_element* next;
};

#endif  // DMCHARGER_UTILITY_STORAGE_FD_H
