#include "rstorage.h"
#include "main.h"
#include "stdlib.h"


#define ADDR_START_MCU_FLASH_MEMORY  0x08000000

static rstorage *first_storage = NULL;

static bool flash_erase(rstorage *instance);

static uint8_t checksum(uint8_t *data, uint32_t size);

#if defined(STM32G474xx)

#define DATA_PORTION_SIZE 8

static uint32_t get_page(uint32_t addr) {
    uint32_t page = 0;

    if (addr < (FLASH_BASE + FLASH_BANK_SIZE))
        page = (addr - FLASH_BASE) / FLASH_PAGE_SIZE;
    else
        page = (addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
    return page;
}

static uint32_t get_bank(uint32_t addr) {
    if (addr < (FLASH_BASE + FLASH_BANK_SIZE))
        return FLASH_BANK_1;
    else
        return FLASH_BANK_2;
}

#elif defined(STM32F103xB)

#define DATA_PORTION_SIZE 4


#endif

bool rstorage_init(rstorage *instance, uint32_t start_address, uint16_t kbytes) {

    if (kbytes == 0 ||
        instance == NULL ||
        start_address < ADDR_START_MCU_FLASH_MEMORY ||
        kbytes > MAX_SIZE_STORAGE_KBYTES)
        return false;

    instance->start_addr = start_address;
#if (FLASH_PAGE_SIZE / 1024 == 1)
    instance->storage_size = kbytes;
#elif (FLASH_PAGE_SIZE / 1024 == 2)
    instance->size = kbytes % 2 == 0 ? kbytes : kbytes + 1;
#elif
    return false;
#endif

    //TODO check strart+size =  the same bank or make double erase in different bank;
    // TODO check the same addr in other storage;

    rstorage** storage = &first_storage;
    while(*storage != NULL) {
        storage = (rstorage**) &((*storage)->next);
    }
    *storage = instance;

    instance->next = NULL;
    instance->state = rstorage_idle;
    return true;
}

static bool flash_erase(rstorage *instance) {

    instance->state = rstorage_erasing;
    instance->data_recorded = false;

#if defined(STM32G474xx)

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    uint32_t page_error = 0;
    FLASH_EraseInitTypeDef erase_struct;
    erase_struct.TypeErase = FLASH_TYPEERASE_PAGES;
    erase_struct.Banks = get_bank(instance->start_addr);
    erase_struct.Page = get_page(instance->start_addr);
    erase_struct.NbPages = instance->size * FLASH_PAGE_SIZE / 1024;

    if (HAL_FLASHEx_Erase(&erase_struct, &page_error) != HAL_OK) {
        instance->state = rstorage_error;
        return false;
    }

#elif defined(STM32F103xB)

    uint32_t page_error = 0;
    FLASH_EraseInitTypeDef erase_struct;
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = storage_addr_start;
    EraseInitStruct.NbPages     = storage_size;

    if(HAL_FLASHEx_Erase(&erase_struct, &page_error) != HAL_OK){
        flash_state = error;
        return false;
    }
#endif

    instance->state = rstorage_idle;
    return true;
}


bool rstorage_write(rstorage *instance, void *data, uint32_t bytes) {

    if (instance == NULL ||
        data == NULL ||
        instance->state != rstorage_idle ||
        instance->size == 0 ||
        bytes > instance->size * 1024 ||
        instance->start_addr < ADDR_START_MCU_FLASH_MEMORY ||
        instance->size > MAX_SIZE_STORAGE_KBYTES)
        return false;

    instance->state = rstorage_writing;

    if (HAL_FLASH_Unlock() != HAL_OK)
        return false;

    if (!flash_erase(instance)) {
        instance->state = rstorage_error;
        return false;
    }

    uint32_t address = instance->start_addr;
    uint32_t portions_of_data = bytes % DATA_PORTION_SIZE == 0 ?
                                bytes / DATA_PORTION_SIZE :
                                bytes / DATA_PORTION_SIZE + 1;

#if defined (STM32G474xx)

    uint64_t *data_in_portions_type = (uint64_t *) data;
    while (address < instance->start_addr + portions_of_data * DATA_PORTION_SIZE) {

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, *data_in_portions_type) != HAL_OK) {
            HAL_FLASH_Lock();
            instance->state = rstorage_error;
            return false;
        }
        data_in_portions_type++;
        address += DATA_PORTION_SIZE;
    }

#elif defined(STM32F103xB)

    uint32_t *data_in_portions_type = (uint32_t *) data;
    while (address < storage_addr_start + portions_of_data * DATA_PORTION_SIZE){

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address,*data_in_portions_type) != HAL_OK) {
            flash_state = error;
            return false;
        }
        data_in_portions_type++;
        address += DATA_PORTION_SIZE;
    }

#endif
    if (HAL_FLASH_Lock() != HAL_OK)
        return false;

    instance->checksum = checksum((uint8_t *) data, bytes);
    instance->data_recorded = true;
    instance->state = rstorage_idle;
    return true;

}


bool rstorage_read(rstorage *instance, void *data, uint32_t bytes) {

    if (instance == NULL ||
        !instance->data_recorded ||
        data == NULL ||
        instance->state != rstorage_idle ||
        instance->size == 0 ||
        bytes > instance->size * 1024 ||
        instance->start_addr < ADDR_START_MCU_FLASH_MEMORY ||
        instance->size > MAX_SIZE_STORAGE_KBYTES)
        return false;

    instance->state = rstorage_reading;

    uint32_t address = instance->start_addr;
    uint32_t *data_in_portions_type = (uint32_t *) data;
    uint32_t portions_of_data = bytes % sizeof(*data_in_portions_type) == 0 ?
                                bytes / sizeof(*data_in_portions_type) :
                                bytes / sizeof(*data_in_portions_type) + 1;

    while (address < instance->start_addr + portions_of_data * sizeof(*data_in_portions_type)) {
        //TODO here place for segmentation if not check bytes last portion data;
        uint32_t index = (address - instance->start_addr) / sizeof(*data_in_portions_type);
        data_in_portions_type[index] = *(__IO uint32_t *) address;
        address += sizeof(*data_in_portions_type);
    }

    uint8_t read_data_chks = checksum((uint8_t *) data, bytes);

    if (read_data_chks != instance->checksum) {
        instance->state = rstorage_error;
        return false;
    }

    instance->state = rstorage_idle;
    return true;

}


static uint8_t checksum(uint8_t *data, uint32_t size) {
    uint8_t chks = 0;

    for (uint32_t i = 0; i < size - 1; i++)
        chks += data[i];

    return -chks;

}


