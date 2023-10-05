#include "rstorage.h"
#include "runit.h"
#include <stdlib.h>

void test_public_api_invalid_arg(void)
{
    rstorage storage1;
    rstorage storage2;
    runit_false(rstorage_init(NULL, 1));
    runit_false(rstorage_init(&storage1, 0));
    runit_false(rstorage_init(&storage2, MAX_SIZE_STORAGE_KBYTES + 1));
}

void test_public_invalid_write_arg(void)
{
    rstorage storage2;
    uint8_t  storage_data[1024];

    // make random data for storage
    for (size_t i = 0; i < sizeof(storage_data); i++)
    {
        storage_data[i] = (uint8_t) (arc4random() % UINT8_MAX);
    }

    runit_true(rstorage_init(&storage2, 1));
    runit_false(rstorage_write(NULL, storage_data, sizeof(storage_data)));
    runit_false(rstorage_write(&storage2, NULL, sizeof(storage_data)));
    runit_true(rstorage_write(&storage2, &storage_data, 0));
    runit_true(rstorage_write(&storage2, (void*) storage_data, sizeof(storage_data)));
    runit_false(rstorage_write(&storage2, (void*) storage_data, sizeof(storage_data) + 1));
    runit_true(rstorage_write(&storage2, (void*) storage_data, sizeof(storage_data) - 1));
}

void test_public_api_invalid_read_arg(void)
{
    rstorage storage3, storage4, storage5, storage6;
    uint8_t  storage_data[1024];

    // make random data for storage
    for (size_t i = 0; i < sizeof(storage_data); i++)
    {
        storage_data[i] = (uint8_t) (arc4random() % UINT8_MAX);
    }

    runit_true(rstorage_init(&storage3, 1));
    runit_true(rstorage_write(&storage3, storage_data, sizeof(storage_data)));
    runit_true(rstorage_read(&storage3, (void*) storage_data, sizeof(storage_data)));

    runit_false(rstorage_read(NULL, &storage_data, sizeof(storage_data)));  // Invalid

    runit_true(rstorage_init(&storage4, 1));
    runit_true(rstorage_write(&storage4, storage_data, sizeof(storage_data)));
    runit_true(rstorage_read(&storage4, (void*) storage_data, sizeof(storage_data)));

    runit_false(rstorage_read(&storage4, NULL, sizeof(storage_data)));  // Invalid

    runit_true(rstorage_init(&storage5, 1));
    runit_true(rstorage_write(&storage5, storage_data, sizeof(storage_data)));
    runit_true(rstorage_read(&storage5, (void*) storage_data, sizeof(storage_data)));

    runit_false(rstorage_read(&storage5, (void*) storage_data, 0));  // Invalid

    runit_true(rstorage_init(&storage6, 1));
    runit_true(rstorage_write(&storage6, storage_data, sizeof(storage_data)));
    runit_true(rstorage_read(&storage6, (void*) storage_data, sizeof(storage_data)));

    runit_false(rstorage_read(&storage6, (void*) storage_data, sizeof(storage_data) + 1));  // Invalid
}

void test_public_api(void)
{
    rstorage storage7;
    uint8_t  storage_data[2048];
    uint8_t  readed_data[2048];

    // make random data for storage
    for (size_t i = 0; i < sizeof(storage_data); i++)
    {
        storage_data[i] = (uint8_t) (arc4random() % UINT8_MAX);
    }

    runit_true(rstorage_init(&storage7, 2));
    runit_true(rstorage_write(&storage7, (void*) storage_data, sizeof(storage_data)));
    runit_true(rstorage_read(&storage7, (void*) readed_data, sizeof(storage_data)));
    runit_true(memcmp(storage_data, readed_data, 2048) == 0);
}

void test_checksum_error(void)
{
    rstorage storage7;
    char     byte = 0x34;
    uint8_t  storage_data[3];
    uint8_t  readed_data[3];

    // make random data for storage
    for (size_t i = 0; i < sizeof(storage_data); i++)
    {
        storage_data[i] = (uint8_t) (arc4random() % UINT8_MAX);
    }

    runit_true(rstorage_init(&storage7, 2));
    runit_true(rstorage_write(&storage7, (void*) storage_data, sizeof(storage_data)));
    FILE* fp;
    fp = fopen("rstorage6.bin", "r+b");
    runit_true(fp != NULL);
    runit_true(fwrite(&byte, 1, 1, fp) == 1);
    fclose(fp);
    runit_false(rstorage_read(&storage7, (void*) readed_data, sizeof(storage_data)));
    runit_true(memcmp(readed_data, storage_data, sizeof(readed_data)) != 0);
}

int main(void)
{
    printf("rstorage test start\n");
    test_public_api_invalid_arg();
    test_public_api_invalid_read_arg();
    test_public_invalid_write_arg();
    test_public_api();


    runit_report();
    return runit_at_least_one_fail;
}
