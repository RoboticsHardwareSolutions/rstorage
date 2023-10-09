#include "rstorage.h"
#include "storage.h"
#include "storage_api.h"

bool rstorage_init(rstorage* instance, int size_kbytes)
{
    if (size_kbytes == 0 || size_kbytes > MAX_SIZE_STORAGE_KBYTES || instance == NULL)
        return false;

    return storage_init(instance, size_kbytes);
}

bool rstorage_write(rstorage* instance, void* data, uint32_t bytes)
{
    if (instance == NULL || data == NULL)
        return false;

    return storage_write(instance, data, bytes);
}

bool rstorage_read(rstorage* instance, void* data, uint32_t bytes)
{
    if (instance == NULL || data == NULL)
        return false;

    return storage_read(instance, data, bytes);
}
