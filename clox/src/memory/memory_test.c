#include "../test_framework/assert.h"
#include "../test_framework/testing.h"
#include "../test_mocks/memory_mocks.h"
#include "../test_mocks/stdlib_mocks.h"
#include "memory.h"

TEST_SUITE(memory)

TEST(memory, reallocate_success)
{
    void* fake_ptr = (void*)0x9999;
    reset_memory_mocks(fake_ptr);
    reset_stdlib_mocks();

    void* result = reallocate(NULL, 0, 64);

    ASSERT_EQ(1, g_mock_realloc_called);
    ASSERT_EQ(0, g_mock_free_called);
    ASSERT_EQ(0, g_mock_exit_called);
    ASSERT_EQ(fake_ptr, result);
}

TEST(memory, reallocate_failure)
{
    reset_memory_mocks(NULL);
    reset_stdlib_mocks();

    if (setjmp(mock_exit_env) == 0) {
        reallocate(NULL, 0, 64);
        ASSERT_FAIL("Should have called exit");
    } else {
        ASSERT_EQ(1, g_mock_exit_called);
        ASSERT_EQ(1, mock_exit_code);
    }
}

TEST(memory, reallocate_failure2)
{
    reset_memory_mocks(NULL);
    reset_stdlib_mocks();

    ASSERT_EXIT_CODE(1, reallocate(NULL, 0, 64));
}