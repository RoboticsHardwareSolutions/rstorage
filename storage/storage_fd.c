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
    int index = 0;

    rstorage** storage = &first_storage;

    while (*storage != NULL)
    {
        index++;
        storage = (rstorage**) &((*storage)->next);
    }

    instance->size          = size_kbytes;
    instance->storage_index = index;
    instance->next          = NULL;
    instance->state         = rstorage_idle;
    instance->checksum      = 0;
    memset(instance->filename, '\0', MAX_FILE_NAME_LENGHT);
    sprintf(instance->filename, "rstorage%d.bin", instance->storage_index);
    *storage = instance;
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

    if(fclose(fp) == EOF)
    {
        instance->state = rstorage_error;
        return false;
    }
    return true;
}

bool storage_fd_write(rstorage* instance, void* data, uint32_t bytes)
{
    if (instance->state != rstorage_idle || instance->size == 0 || bytes > (uint32_t) instance->size * 1024)
        return false;

    instance->state = rstorage_writing;

    FILE* fp;
    fp = fopen(instance->filename, "r+b");

    if (!fp)
    {
        instance->state = rstorage_error;
        return false;
    }

    if (fwrite(data, 1, bytes, fp) != bytes)
    {
        fclose(fp);
        instance->state = rstorage_error;
        return false;
    }

    instance->checksum = checksum(data,bytes);

    if(fclose(fp) == EOF)
    {
        instance->state = rstorage_error;
        return false;
    }
    instance->data_recorded = true;
    instance->state = rstorage_idle;
    return true;

}

bool storage_fd_read(rstorage* instance, void* data, uint32_t bytes)
{
    if (!instance->data_recorded || instance->state != rstorage_idle || instance->size == 0 ||
        bytes > (uint32_t) instance->size * 1024)
        return false;

    instance->state = rstorage_reading;

    FILE* fp;
    fp = fopen(instance->filename, "r");

    if (!fp)
    {
        instance->state = rstorage_error;
        return false;
    }

    if(fread(data,1,bytes,fp) != bytes)
    {
        fclose(fp);
        instance->state = rstorage_error;
        return false;
    }

    if(checksum(data,bytes) != instance->checksum )
    {
        fclose(fp);
        instance->state = rstorage_error;
        return false;
    }

    if(fclose(fp) == EOF)
    {
        instance->state = rstorage_error;
        return false;
    }

    instance->state = rstorage_idle;
    return true;
}

#endif
