#include "rstorage.h"

static rstorage * first_storage = NULL;



bool rstorage_init(rstorage* instance, int size_kbytes)
{
    if (size_kbytes == 0 ||size_kbytes > MAX_SIZE_STORAGE_KBYTES || instance == NULL)
        return false;
}



bool rstorage_write(rstorage* instance, void* data, uint32_t bytes)
{
    if (instance == NULL || data == NULL )
        return false;



}

bool rstorage_read(rstorage* instance, void* data, uint32_t bytes)
{
    if (instance == NULL  || data == NULL )
        return false;


}
