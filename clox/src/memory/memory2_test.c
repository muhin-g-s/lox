#include <setjmp.h>

#include "../test_framework/assert.h"
#include "../test_framework/testing.h"
#include "../test_mocks/memory_mocks.h"
#include "../test_mocks/stdlib_mocks.h"
#include "memory.h"

TEST_SUITE(memory2)

TEST(memory2, reallocate_success)
{
    // Сбрасываем состояние моков перед тестом
    // reset_memory_mocks(NULL);
    // reset_stdlib_mocks();

    // void* fake_ptr = (void*)0x9999;
    // mock_realloc_result = fake_ptr;

    // void* result = reallocate(NULL, 0, 64);

    // ASSERT_EQ(1, g_mock_realloc_called);
    // ASSERT_EQ(0, g_mock_free_called);
    // ASSERT_EQ(0, g_mock_exit_called);
    // ASSERT_EQ(0, result != fake_ptr);
}

// TEST(memory, reallocate_failure)
// {
//     reset_memory_mocks();
//     reset_stdlib_mocks();

//     // Симулируем ошибку realloc
//     mock_realloc_result = NULL;

//     // Устанавливаем точку возврата для longjmp
//     if (setjmp(mock_exit_env) == 0) {
//         reallocate(NULL, 0, 64);
//         ASSERT_FAIL("Should have called exit");
//     } else {
//         // Проверяем, что exit был вызван с правильным кодом
//         ASSERT_EQ(1, g_mock_exit_called);
//         ASSERT_EQ(1, mock_exit_code);
//     }
// }