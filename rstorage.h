#ifndef __RSTORAGE_H_
#define __RSTORAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdio.h"
#include "stdint.h"
#include "rstorage_def.h"

typedef struct storage_element rstorage;

#ifndef RSTORAGE_USING_FD

/** NOTES USING ONLY FOR MCU
 * @param instance -   instance of storage
 * @param start_address - address of flash memory MCU where save data
 * @return true if successfully configured
 */
bool rstorage_config_flash_memory(rstorage* instance, uint32_t start_address);

#endif //RSTORAGE_USING_FD

/**
 * @param instance  - instance of storage
 * @param type - type of storage
 * @param size_kbytes - size of storage in kbytes
 *         NOTES !!! #define MAX_SIZE_STORAGE_KBYTES 256
 * @return - true if successfully inited
 */
bool rstorage_init(rstorage* instance, int size_kbytes);


/**
 * @param instance
 * @param data
 * @param bytes
 * @return
 */
bool rstorage_write(rstorage* instance, void* data, uint32_t bytes);

/**
 * @param instance
 * @param data
 * @param bytes
 * @return
 */

bool rstorage_read(rstorage* instance, void* data, uint32_t bytes);

#ifdef __cplusplus
}
#endif

#endif
