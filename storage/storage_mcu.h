#ifndef __STORAGE_MCU_H_
#define __STORAGE_MCU_H_

#include "rstorage_def.h"
#include "stdint.h"
#include "stdbool.h"

struct storage_element
{
    rstorage_state          state;
    uint32_t                start_addr;
    uint32_t                size;
    bool                    data_recorded;
    uint8_t                 checksum;
    struct storage_element* next;
};

#endif  //__STORAGE_MCU_H_
