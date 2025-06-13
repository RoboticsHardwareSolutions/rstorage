#if defined(RSTORAGE_USING_FD)

#    include "storage.h"
#    include "string.h"
#    include <stdio.h>

static struct storage_element* first_storage = NULL;

bool storage_init(struct storage_element* instance, int size_kbytes)
{
    instance->size          = size_kbytes;
    instance->storage_index = 0;
    instance->next          = NULL;
    instance->state         = storage_idle;

    struct storage_element** storage = &first_storage;
    while (*storage != NULL)
    {
        instance->storage_index++;
        storage = (struct storage_element**) &((*storage)->next);
    }
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
        instance->state = storage_error;
        return false;
    }

    if (fclose(fp) == EOF)
    {
        instance->state = storage_error;
        return false;
    }
    return true;
}

bool storage_write(struct storage_element* instance, void* data, uint32_t bytes)
{
    if (instance->state != storage_idle || instance->size == 0 || bytes > (uint32_t) instance->size * 1024)
        return false;

    instance->state = storage_writing;

    FILE* fp;
    fp = fopen(instance->filename, "r+b");

    if (!fp)
    {
        instance->state = storage_error;
        return false;
    }

    if (fwrite(data, 1, bytes, fp) != bytes)
    {
        fclose(fp);
        instance->state = storage_error;
        return false;
    }

    if (fclose(fp) == EOF)
    {
        instance->state = storage_error;
        return false;
    }

    instance->state = storage_idle;
    return true;
}

bool storage_read(struct storage_element* instance, void* data, uint32_t bytes)
{
    if (instance->state != storage_idle || instance->size == 0 || bytes > (uint32_t) instance->size * 1024 ||
        bytes == 0)
        return false;

    instance->state = storage_reading;

    FILE* fp;
    fp = fopen(instance->filename, "r");

    if (!fp)
    {
        instance->state = storage_error;
        return false;
    }

    if (fread(data, 1, bytes, fp) != bytes)
    {
        fclose(fp);
        instance->state = storage_error;
        return false;
    }

    if (fclose(fp) == EOF)
    {
        instance->state = storage_error;
        return false;
    }

    instance->state = storage_idle;
    return true;
}

#endif
