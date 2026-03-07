#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>
#include <stdbool.h>
#include <setjmp.h>

#include "../test_mocks/stdlib_mocks.h"

extern int g_tests_failed;

#define EQ(e,a, isFatal)                                 \
    do {                                                 \
        __typeof__ (e) _e = (e);                         \
        __typeof__ (a) _a = (a);                         \
        if (_e != _a) {                                  \
            g_tests_failed++;                            \
            printf("FAIL %s:%d: expected %d, got %d\n", __FILE__, __LINE__, _e, _a); \
            if (isFatal) return;                         \
            else break;                                  \
        }                                                \
    } while(0)

#define ASSERT_EQ(e,a) EQ(e, a, true)
#define EXPECT_EQ(e,a) EQ(e, a, false)

#define CHECK_NULL(p, operand, isFatal)                  \
    do {                                                 \
        if (!((p) operand NULL)) {                          \
            g_tests_failed++;                            \
            printf("FAIL %s:%d: expected value %s NULL\n", __FILE__, __LINE__, #operand); \
            if (isFatal) return;                         \
            else break;                                  \
        }                                                \
    } while(0)

#define ASSERT_NULL(p) CHECK_NULL(p, ==, true)
#define EXPECT_NULL(p) CHECK_NULL(p, ==, false)
#define ASSERT_NOT_NULL(p) CHECK_NULL(p, !=, true)
#define EXPECT_NOT_NULL(p) CHECK_NULL(p, !=, false)

#define ASSERT_FAIL(msg)                                \
    do {                                                \
        g_tests_failed++;                               \
        printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, msg); \
        return;                                         \
    } while(0)

#define ASSERT_EXIT_CODE(expected_code, code)           \
    do {                                                \
        if (setjmp(mock_exit_env) == 0) {                          \
            code;                                       \
            ASSERT_FAIL("Should have called exit");     \
        } else {                                        \
            ASSERT_EQ(1, g_mock_exit_called);           \
            ASSERT_EQ(expected_code, mock_exit_code);   \
        }                                               \
    } while(0)

#endif
