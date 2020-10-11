#ifndef __RSTORAGE_H_
#define __RSTORAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdio.h"
#include "stdint.h"


#define MAX_SIZE_STORAGE_KBYTES    256

typedef enum {
    rstorage_idle,
    rstorage_writing,
    rstorage_reading,
    rstorage_erasing,
    rstorage_error,
} rstorage_state;


typedef struct {
    rstorage_state state;
    uint32_t start_addr;
    uint32_t size;
    bool data_recorded;
    uint8_t checksum;
    struct rstorage *next;
} rstorage;


bool rstorage_init(rstorage *instance, uint32_t start_address, uint16_t kbytes);

bool rstorage_write(rstorage *instance, void *data, uint32_t bytes);

bool rstorage_read(rstorage *instance, void *data, uint32_t bytes);


#ifdef __cplusplus
}
#endif

#endif
