# rstorage

simple interface for read & write ROM on linux / mac / stm32

supported mcu : `STM32G474xx` , `STM32F103xB` , .... 

```
#add to cmake :

include(rstorage/rstorage.cmake)

include_directories( ... ${RSTORAGE_DIRECTORIES})

add_executable( ... ${RSTORAGE_EXECUTABLE_FILES})

```

```
typedef struct {
    rstorage_state state;
    uint32_t start_addr;
    uint32_t size;
    bool data_recorded;
    uint8_t checksum;
    struct rstorage *next;
} rstorage;

bool rstorage_init(rstorage *instance, uint32_t start_address, uint16_t kbytes);

bool rstorage_write(rstorage *instance, void *data, uint32_t size);

bool rstorage_read(rstorage *instance, void *data, uint32_t size);

```

example :

```
#define ADDR_FLASH_PAGE_124   ((uint32_t)0x0803E000)

    rstorage storage = {0};
    uint8_t app_data[1024];
    
    for (uint32_t i = 0; i < 1024 * 1; i++) {
        app_data[i] = i % UINT8_MAX;
    }

    rstorage_init(&storage, ADDR_FLASH_PAGE_124, 1);

    for (;;) {
        if(!rstorage_write(&storage, (void *) app_data, 1024 * 1)){
            led_red_on();
            while(1){};
        }
        osDelay(1);
        if(!rstorage_read(&storage, (void *) app_data, 1024 * 1)){
            led_red_on();
            while(1){};
        }
        osDelay(1);
    }
#undef ADDR_FLASH_PAGE_124

```
