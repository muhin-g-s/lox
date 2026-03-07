#include "../test_framework/assert.h"
#include "../test_framework/testing.h"
#include "../test_mocks/memory_mocks.h"
#include "../test_mocks/stdlib_mocks.h"
#include "value.h"

TEST_SUITE(ValueArray)

TEST(ValueArray, initValueArray) {
    ValueArray valueArray;
    
    initValueArray(&valueArray);

    EXPECT_EQ(0, valueArray.capacity);
    EXPECT_EQ(0, valueArray.count);
    EXPECT_NULL(valueArray.values);
}

#define MEMORY_SIZE 10

TEST(ValueArray, writeValueArrayNoAlloc) {
    setupMemoryMocks();

    ValueArray valueArray;
    Value values[MEMORY_SIZE];
    valueArray.capacity = MEMORY_SIZE;
    valueArray.values = values;
    valueArray.count = 0;
    Value testValue = CREATE_VALUE(0.0);

    writeValueArray(&valueArray, testValue);

    EXPECT_EQ(0, g_mock_realloc_called);
    EXPECT_EQ(MEMORY_SIZE, valueArray.capacity);
    EXPECT_EQ(1, valueArray.count);
    EXPECT_TRUE(valuesEqual(valueArray.values[0], testValue));
}

TEST(ValueArray, writeValueArrayAlloc) {
    Value blok[MEMORY_SIZE];
    #define SIZE 1
    Value* memmory[SIZE] = {&blok};
    setupMemoryMocksWithMemory(memmory, SIZE);

    ValueArray valueArray;
    initValueArray(&valueArray);

    Value testValue = CREATE_VALUE(0.0);

    writeValueArray(&valueArray, testValue);

    EXPECT_EQ(1, g_mock_realloc_called);
    EXPECT_EQ(1, valueArray.count);
    EXPECT_MORE_THAN(valueArray.capacity, 0);
    EXPECT_TRUE(valuesEqual(valueArray.values[0], testValue));
}

TEST(ValueArray, freeValueArray) {
    setupMemoryMocks(NULL);

    ValueArray valueArray;
    Value values[MEMORY_SIZE];
    valueArray.capacity = MEMORY_SIZE;
    valueArray.values = values;
    valueArray.count = 10;

    freeValueArray(&valueArray);

    EXPECT_EQ(1, g_mock_free_called);
    
    EXPECT_EQ(0, valueArray.capacity);
    EXPECT_EQ(0, valueArray.count);
    EXPECT_NULL(valueArray.values);
}