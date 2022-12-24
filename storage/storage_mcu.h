#if defined(STM32G474xx) || defined(STM32F103xB)

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

typedef struct storage_element rstorage;

bool storage_mcu_init(rstorage * instance, int size_kbytes);
bool storage_mcu_write(rstorage* instance, void* data, uint32_t bytes);
bool storage_mcu_read(rstorage* instance, void* data, uint32_t bytes);

#endif  //__STORAGE_MCU_H_
#endif  //defined(STM32G474xx) || defined(STM32F103xB)
