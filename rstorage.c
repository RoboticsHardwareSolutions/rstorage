#include "rstorage.h"
#include "storage.h"

bool rstorage_init(rstorage* instance, int size_kbytes)
{
    if (size_kbytes == 0 || size_kbytes > MAX_SIZE_STORAGE_KBYTES || instance == NULL)
        return false;

#if defined(RSTORAGE_USING_FD)
    return storage_init(instance, size_kbytes);
#endif

#if defined(STM32G474xx) || defined(STM32F103xB)
    return storage_init(instance, size_kbytes);
#endif
}

bool rstorage_write(rstorage* instance, void* data, uint32_t bytes)
{
    if (instance == NULL || data == NULL)
        return false;

#if defined(RSTORAGE_USING_FD)
    return storage_write(instance, data, bytes);
#endif

#if defined(STM32G474xx) || defined(STM32F103xB)
    return storage_write(instance, data, bytes);
#endif
}

bool rstorage_read(rstorage* instance, void* data, uint32_t bytes)
{
    if (instance == NULL || data == NULL)
        return false;

#if defined(RSTORAGE_USING_FD)
    return storage_read(instance, data, bytes);
#endif

#if defined(STM32G474xx) || defined(STM32F103xB)
    return storage_read(instance, data, bytes);
#endif
}
