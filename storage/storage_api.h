#include "storage.h"

bool storage_init(struct storage_element* instance, int size_kbytes);

bool storage_write(struct storage_element* instance, void* data, uint32_t bytes);

bool storage_read(struct storage_element* instance, void* data, uint32_t bytes);
