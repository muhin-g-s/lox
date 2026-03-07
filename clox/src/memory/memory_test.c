#include "../test_framework/assert.h"
#include "../test_framework/testing.h"
#include "../test_mocks/memory_mocks.h"
#include "../test_mocks/stdlib_mocks.h"
#include "memory.h"

TEST_SUITE(Memory)

TEST(Memory, reallocate_zero_size_frees_pointer) {
    int dummy;

    reset_memory_mocks(NULL);

    void* result = reallocate(&dummy, sizeof(int), 0);

    EXPECT_NULL(result);
    EXPECT_EQ(1, g_mock_free_called);
    EXPECT_EQ(0, g_mock_realloc_called);
}

TEST(Memory, reallocate_success_returns_new_pointer) {
    int dummy;
    int newBlock;

    reset_memory_mocks(&newBlock);

    void* result = reallocate(&dummy, sizeof(int), sizeof(int) * 2);

    EXPECT_EQ(&newBlock, result);
    EXPECT_EQ(1, g_mock_realloc_called);
    EXPECT_EQ(0, g_mock_free_called);
}

TEST(Memory, reallocate_failure_exits) {
    int dummy;

    reset_memory_mocks(NULL);
    reset_stdlib_mocks();

    ASSERT_EXIT_CODE(1, reallocate(&dummy, sizeof(int), sizeof(int) * 2));

    EXPECT_EQ(1, g_mock_realloc_called);
}
