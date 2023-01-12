#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include "rstorage.h"
#include "stdio.h"

TEST_CASE("Public API ")
{

/**********************************************************************************************************************/
    SECTION("invalid arg tests for init function")
    {
        rstorage storage1;
        rstorage storage2;
        REQUIRE(rstorage_init(NULL, 1) == false);
        REQUIRE(rstorage_init(&storage1, 0) == false);
        REQUIRE(rstorage_init(&storage2, MAX_SIZE_STORAGE_KBYTES + 1) == false);
    }
/**********************************************************************************************************************/
    SECTION("invalid arg tests for write")
    {
        rstorage storage2;
        uint8_t  storage_data[1024];

        // make random data for storage
        for (size_t i = 0; i < sizeof(storage_data); i++)
        {
            storage_data[i] = (uint8_t) (arc4random() % UINT8_MAX);
        }


        REQUIRE(rstorage_init(&storage2, 1) == true);
        REQUIRE(rstorage_write(NULL, storage_data, sizeof(storage_data)) == false);
        REQUIRE(rstorage_write(&storage2, NULL, sizeof(storage_data)) == false);
        REQUIRE(rstorage_write(&storage2, &storage_data, 0) == true);
        REQUIRE(rstorage_write(&storage2, (void*) storage_data, sizeof(storage_data)) == true);
        REQUIRE(rstorage_write(&storage2, (void*) storage_data, sizeof(storage_data) + 1) == false);
        REQUIRE(rstorage_write(&storage2, (void*) storage_data, sizeof(storage_data) - 1) == true);
    }
/**********************************************************************************************************************/
    SECTION("invalid arg tests for read")
    {
        rstorage storage3, storage4, storage5, storage6;
        uint8_t  storage_data[1024];

        // make random data for storage
        for (size_t i = 0; i < sizeof(storage_data); i++)
        {
            storage_data[i] = (uint8_t) (arc4random() % UINT8_MAX);
        }

        REQUIRE(rstorage_init(&storage3, 1) == true);
        REQUIRE(rstorage_write(&storage3,storage_data,sizeof(storage_data)) == true);
        REQUIRE(rstorage_read(&storage3, (void*) storage_data, sizeof(storage_data)) == true);

        REQUIRE(rstorage_read(NULL, &storage_data, sizeof(storage_data)) == false); // Invalid

        REQUIRE(rstorage_init(&storage4, 1) == true);
        REQUIRE(rstorage_write(&storage4,storage_data,sizeof(storage_data)) == true);
        REQUIRE(rstorage_read(&storage4, (void*) storage_data, sizeof(storage_data)) == true);

        REQUIRE(rstorage_read(&storage4, NULL, sizeof(storage_data)) == false);  // Invalid

        REQUIRE(rstorage_init(&storage5, 1) == true);
        REQUIRE(rstorage_write(&storage5,storage_data,sizeof(storage_data)) == true);
        REQUIRE(rstorage_read(&storage5, (void*) storage_data, sizeof(storage_data)) == true);

        REQUIRE(rstorage_read(&storage5, (void*) storage_data, 0) == false); // Invalid


        REQUIRE(rstorage_init(&storage6, 1) == true);
        REQUIRE(rstorage_write(&storage6,storage_data,sizeof(storage_data)) == true);
        REQUIRE(rstorage_read(&storage6, (void*) storage_data, sizeof(storage_data)) == true);

        REQUIRE(rstorage_read(&storage6, (void*) storage_data, sizeof(storage_data) + 1) == false); // Invalid

    }
/**********************************************************************************************************************/
    SECTION("test public API functions")
    {
        rstorage storage7;
        uint8_t  storage_data[2048];
        uint8_t  readed_data[2048];

        // make random data for storage
        for (size_t i = 0; i < sizeof(storage_data); i++)
        {
            storage_data[i] = (uint8_t) (arc4random() % UINT8_MAX);
        }

        REQUIRE(rstorage_init(&storage7, 2) == true);
        REQUIRE(rstorage_write(&storage7,(void *)storage_data, sizeof(storage_data)) == true);
        REQUIRE(rstorage_read(&storage7,(void *)readed_data, sizeof(storage_data)) == true);
        REQUIRE(memcmp(storage_data,readed_data,2048) == 0);
    }
/**********************************************************************************************************************/
    SECTION("test checksum error")
    {
        rstorage storage7;
        char byte  = 0x34;
        uint8_t  storage_data[3];
        uint8_t  readed_data[3];

        // make random data for storage
        for (size_t i = 0; i < sizeof(storage_data); i++)
        {
            storage_data[i] = (uint8_t) (arc4random() % UINT8_MAX);
        }

        REQUIRE(rstorage_init(&storage7, 2) == true);
        REQUIRE(rstorage_write(&storage7,(void *)storage_data, sizeof(storage_data)) == true);
        FILE* fp;
        fp = fopen("rstorage6.bin", "r+b");
        REQUIRE(fp != NULL);
        REQUIRE(fwrite(&byte, 1, 1, fp) == 1);
        fclose(fp);
        REQUIRE(rstorage_read(&storage7,(void *)readed_data, sizeof(storage_data)) == false);
        REQUIRE(memcmp(readed_data,storage_data, sizeof(readed_data)) != 0);

    }
}


