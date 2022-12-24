#include "rstorage.h"



bool rstorage_init(rstorage* instance, rstorage_type type, int size_kbytes)
{
    if (size_kbytes == 0 || instance == NULL || type < ADDR_START_MCU_FLASH_MEMORY ||
        size_kbytes > MAX_SIZE_STORAGE_KBYTES)
        return false;

    instance->start_addr = type;
#if (FLASH_PAGE_SIZE / 1024 == 1)
    instance->storage_size = kbytes;
#elif (FLASH_PAGE_SIZE / 1024 == 2)
    instance->size = kbytes % 2 == 0 ? kbytes : kbytes + 1;
#elif
    return false;
#endif

    // TODO check strart+size =  the same bank or make double erase in different bank;
    //  TODO check the same addr in other storage;

    rstorage** storage = &first_storage;
    while (*storage != NULL)
    {
        storage = (rstorage**) &((*storage)->next);
    }
    *storage = instance;

    instance->next  = NULL;
    instance->state = rstorage_idle;
    return true;
}



bool rstorage_write(rstorage* instance, void* data, uint32_t bytes)
{
    if (instance == NULL || data == NULL || instance->state != rstorage_idle || instance->size == 0 ||
        bytes > instance->size * 1024 || instance->start_addr < ADDR_START_MCU_FLASH_MEMORY ||
        instance->size > MAX_SIZE_STORAGE_KBYTES)
        return false;

    instance->state = rstorage_writing;

    if (HAL_FLASH_Unlock() != HAL_OK)
        return false;

    if (!flash_erase(instance))
    {
        instance->state = rstorage_error;
        return false;
    }

    uint32_t address = instance->start_addr;
    uint32_t portions_of_data =
        bytes % DATA_PORTION_SIZE == 0 ? bytes / DATA_PORTION_SIZE : bytes / DATA_PORTION_SIZE + 1;

#if defined(STM32G474xx)

    uint64_t* data_in_portions_type = (uint64_t*) data;
    while (address < instance->start_addr + portions_of_data * DATA_PORTION_SIZE)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, *data_in_portions_type) != HAL_OK)
        {
            HAL_FLASH_Lock();
            instance->state = rstorage_error;
            return false;
        }
        data_in_portions_type++;
        address += DATA_PORTION_SIZE;
    }

#elif defined(STM32F103xB)

    uint32_t* data_in_portions_type = (uint32_t*) data;
    while (address < storage_addr_start + portions_of_data * DATA_PORTION_SIZE)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, *data_in_portions_type) != HAL_OK)
        {
            flash_state = error;
            return false;
        }
        data_in_portions_type++;
        address += DATA_PORTION_SIZE;
    }

#endif
    if (HAL_FLASH_Lock() != HAL_OK)
        return false;

    instance->checksum      = checksum((uint8_t*) data, bytes);
    instance->data_recorded = true;
    instance->state         = rstorage_idle;
    return true;
}

bool rstorage_read(rstorage* instance, void* data, uint32_t bytes)
{
    if (instance == NULL || !instance->data_recorded || data == NULL || instance->state != rstorage_idle ||
        instance->size == 0 || bytes > instance->size * 1024 || instance->start_addr < ADDR_START_MCU_FLASH_MEMORY ||
        instance->size > MAX_SIZE_STORAGE_KBYTES)
        return false;

    instance->state = rstorage_reading;

    uint32_t  address               = instance->start_addr;
    uint32_t* data_in_portions_type = (uint32_t*) data;
    uint32_t  portions_of_data      = bytes % sizeof(*data_in_portions_type) == 0
                                          ? bytes / sizeof(*data_in_portions_type)
                                          : bytes / sizeof(*data_in_portions_type) + 1;

    while (address < instance->start_addr + portions_of_data * sizeof(*data_in_portions_type))
    {
        // TODO here place for segmentation if not check bytes last portion data;
        uint32_t index               = (address - instance->start_addr) / sizeof(*data_in_portions_type);
        data_in_portions_type[index] = *(__IO uint32_t*) address;
        address += sizeof(*data_in_portions_type);
    }

    uint8_t read_data_chks = checksum((uint8_t*) data, bytes);

    if (read_data_chks != instance->checksum)
    {
        instance->state = rstorage_error;
        return false;
    }

    instance->state = rstorage_idle;
    return true;
}
