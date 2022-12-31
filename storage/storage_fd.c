#if defined(RSTORAGE_USING_FD)

#    include "storage_fd.h"
#    include "stdint.h"
#    include "string.h"
#    include "stdlib.h"
#    include <errno.h>
#    include <stdlib.h>
#    include <stdio.h>

static rstorage* first_storage = NULL;

bool storage_fd_init(rstorage* instance, int size_kbytes)
{
    bool first = false;
    int index = 0;

    if (first_storage == NULL)
        first = true;

    rstorage** storage = &first_storage;

    while (*storage != NULL)
    {
        if(first)
            instance->storage_index = 0;

        index   = (*storage)->storage_index;
        storage = (rstorage**) &((*storage)->next);
    }

    *storage                = instance;
    instance->size          = size_kbytes;
    instance->storage_index = index++;
    instance->next          = NULL;
    instance->state         = rstorage_idle;
    instance->checksum      = 0;
    memset(instance->filename, '\0', MAX_FILE_NAME_LENGHT);
    sprintf(instance->filename, "rstorage%d.bin", instance->storage_index);
    FILE* fp;

    fp = fopen(instance->filename, "r+b");

    if (!fp)
        fp = fopen(instance->filename, "w+b");

    if (!fp)
    {
        fclose(fp);
        instance->state = rstorage_error;
        return false;
    }
    fclose(fp);
    return true;
}

bool storage_fd_write(rstorage* instance, void* data, uint32_t bytes)
{
    return true;
}

bool storage_fd_read(rstorage* instance, void* data, uint32_t bytes)
{
    FILE* fp;
    fp = fopen(instance->filename, "r");
    if (!fp)
        return false;

    fclose(fp);
    return true;
}

#endif
