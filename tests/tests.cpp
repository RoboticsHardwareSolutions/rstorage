#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include "rstorage.h"

TEST_CASE("Public API ")
{
    SECTION("invalid arg tests for init function")
    {
        rstorage storage2;
        REQUIRE(rstorage_init(NULL, 1) == false);
        REQUIRE(rstorage_init(&storage2, 0) == false);
        REQUIRE(rstorage_init(&storage2, MAX_SIZE_STORAGE_KBYTES + 1) == false);
    }
    SECTION("invalid arg tests for write")
    {
        rstorage storage2;
        uint8_t  storage_data[1024];

        // make random data for storage
        for (size_t i = 0; i < sizeof(storage_data); i++)
        {
            storage_data[i] = (uint8_t) (arc4random() % UINT8_MAX);
        }

        // calculate checksumm
        uint8_t chks = checksum(storage_data, sizeof(storage_data));

        REQUIRE(rstorage_init(&storage2, 1) == true);
        REQUIRE(rstorage_write(NULL, &storage_data, sizeof(storage_data)) == false);
        REQUIRE(rstorage_write(&storage2, NULL, sizeof(storage_data)) == false);
        REQUIRE(rstorage_write(&storage2, &storage_data, 0) == true);
        REQUIRE(rstorage_write(&storage2, (void*) &storage_data, sizeof(storage_data)) == true);
        REQUIRE(rstorage_write(&storage2, (void*) &storage_data, sizeof(storage_data) + 1) == false);
        REQUIRE(rstorage_write(&storage2, (void*) &storage_data, sizeof(storage_data) - 1) == true);
    }

    SECTION("init read write")
    {
        //        rstorage storage0;
        //        rstorage storage1;
        //        rstorage storage2;
        //        REQUIRE(rstorage_init(&storage0, 1) == true);
        //        REQUIRE(rstorage_init(&storage1, 1) == true);
        //        REQUIRE(rstorage_init(&storage2, 1) == true);
    }
}

/**********************************************************************************************************************/
