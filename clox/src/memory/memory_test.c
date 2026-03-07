#include "../test_framework/assert.h"
#include "../test_framework/testing.h"
#include "../test_mocks/memory_mocks.h"
#include "../test_mocks/stdlib_mocks.h"
#include "memory.h"

TEST_SUITE(Memory)

TEST(Memory, grow_capacity_below_min_returns_min) {
    ASSERT_EQ(MIN_CAPACITY, GROW_CAPACITY(0));
    ASSERT_EQ(MIN_CAPACITY, GROW_CAPACITY(1));
    ASSERT_EQ(MIN_CAPACITY, GROW_CAPACITY(4));
    ASSERT_EQ(MIN_CAPACITY, GROW_CAPACITY(7));
}

TEST(Memory, grow_capacity_at_min_doubles) {
    ASSERT_EQ(MIN_CAPACITY * 2, GROW_CAPACITY(MIN_CAPACITY));
}

TEST(Memory, grow_capacity_above_min_doubles) {
    ASSERT_EQ(16, GROW_CAPACITY(8));
    ASSERT_EQ(32, GROW_CAPACITY(16));
    ASSERT_EQ(100, GROW_CAPACITY(50));
}

TEST(Memory, grow_capacity_does_not_evaluate_side_effects_twice) {
    int counter = 0;
    #define INCREMENT_AND_CAP(x) (++(counter), (x))

    int result = GROW_CAPACITY(INCREMENT_AND_CAP(MIN_CAPACITY - 1));

    ASSERT_EQ(MIN_CAPACITY, result);
    ASSERT_EQ(1, counter);
}

TEST(Memory, grow_array_allocates_new_buffer) {
    int dummy;
    int newBlock1, newBlock2;
    int* initial_ptr = &dummy;

    void* memory[] = {&newBlock1, &newBlock2};
    setupMemoryMocksWithMemory(memory, 2);

    int* result = GROW_ARRAY(int, initial_ptr, 0, MIN_CAPACITY);

    ASSERT_NOT_NULL(result);
    ASSERT_EQ(1, g_mock_realloc_called);
}

TEST(Memory, grow_array_grows_existing_array) {
    int dummy;
    int newBlock;
    int* initial_ptr = &dummy;

    void* memory[] = {&newBlock};
    setupMemoryMocksWithMemory(memory, 1);

    int* result = GROW_ARRAY(int, initial_ptr, 4, 8);

    ASSERT_NOT_NULL(result);
    ASSERT_EQ(1, g_mock_realloc_called);
}

TEST(Memory, grow_array_calculates_correct_sizes) {
    int dummy;
    int newBlock;
    int* initial_ptr = &dummy;

    void* memory[] = {&newBlock};
    setupMemoryMocksWithMemory(memory, 1);

    int* result = GROW_ARRAY(int, initial_ptr, 0, 8);

    ASSERT_NOT_NULL(result);
    ASSERT_EQ(1, g_mock_realloc_called);
}

TEST(Memory, grow_array_works_with_different_types) {
    int dummy_int;
    double dummy_double;
    int newIntBlock;
    double newDoubleBlock;

    void* int_memory[] = {&newIntBlock};
    void* double_memory[] = {&newDoubleBlock};

    setupMemoryMocksWithMemory(int_memory, 1);
    int* int_result = GROW_ARRAY(int, &dummy_int, 0, 4);
    ASSERT_NOT_NULL(int_result);

    setupMemoryMocksWithMemory(double_memory, 1);
    double* double_result = GROW_ARRAY(double, &dummy_double, 0, 4);
    ASSERT_NOT_NULL(double_result);
}

TEST(Memory, free_array_frees_memory) {
    int dummy;

    setupMemoryMocks();

    FREE_ARRAY(int, &dummy, 4);

    ASSERT_EQ(1, g_mock_free_called);
    ASSERT_EQ(0, g_mock_realloc_called);
}

TEST(Memory, free_array_calculates_correct_size) {
    int dummy;

    setupMemoryMocks();

    FREE_ARRAY(int, &dummy, 10);

    ASSERT_EQ(1, g_mock_free_called);
}

TEST(Memory, free_array_works_with_different_types) {
    int dummy_int;
    double dummy_double;

    setupMemoryMocks();
    FREE_ARRAY(int, &dummy_int, 4);
    ASSERT_EQ(1, g_mock_free_called);

    setupMemoryMocks();
    FREE_ARRAY(double, &dummy_double, 4);
    ASSERT_EQ(1, g_mock_free_called);
}

TEST(Memory, reallocate_zero_size_frees_pointer) {
    int dummy;

    setupMemoryMocks();

    void* result = reallocate(&dummy, sizeof(int), 0);

    EXPECT_NULL(result);
    EXPECT_EQ(1, g_mock_free_called);
    EXPECT_EQ(0, g_mock_realloc_called);
}

TEST(Memory, reallocate_success_returns_new_pointer) {
    int dummy;
    #define SIZE 1
    int newBlock;

    int* memory[SIZE] = {&newBlock};

    setupMemoryMocksWithMemory(&memory, SIZE);

    void* result = reallocate(&dummy, sizeof(int), sizeof(int) * 2);

    EXPECT_EQ(&newBlock, result);
    EXPECT_EQ(1, g_mock_realloc_called);
    EXPECT_EQ(0, g_mock_free_called);
}

TEST(Memory, reallocate_failure_exits) {
    int dummy;

    setupMemoryMocks();
    reset_stdlib_mocks();

    ASSERT_EXIT_CODE(1, reallocate(&dummy, sizeof(int), sizeof(int) * 2));

    EXPECT_EQ(1, g_mock_realloc_called);
}
