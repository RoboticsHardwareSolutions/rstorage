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
    struct rstorage *next;
} rstorage;

bool rstorage_init(rstorage *instance, uint32_t start_address, uint16_t kbytes);

bool rstorage_write(rstorage *instance, void *data, uint32_t size);

bool rstorage_read(rstorage *instance, void *data, uint32_t size);

```

example :

```
    RLOG_INFO("start interface");
    uint8_t  app_data[256];
    rstorage storage;
    rstorage_config_flash_memory(&storage, APP_DATA_ADDR);
    for (uint32_t i = 0; i < sizeof(app_data); i++)
    {
        app_data[i] = i % UINT8_MAX;
    }
    rstorage_init(&storage, APP_DATA_SIZE);

    for (;;)
    {
        taskENTER_CRITICAL();
        if (!rstorage_write(&storage, (void*) app_data, sizeof(app_data)))
        {
            button_led_turn_on();
        }
        taskEXIT_CRITICAL();
        osDelay(1);
        taskENTER_CRITICAL();
        if (!rstorage_read(&storage, (void*) app_data, sizeof(app_data)))
        {
            button_led_turn_on();
            while (1)
            {
            };
        }
        taskEXIT_CRITICAL();
        osDelay(1);
    }

```
