#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>
#include <stdbool.h>
#include <setjmp.h>

#include "../test_mocks/stdlib_mocks.h"

extern int g_tests_failed;

void assert_fail(const char* file, int line, const char* msg);

#define CHECK(expr, isFatal, msg)                        \
    do {                                                 \
        if (!(expr)) {                                   \
            assert_fail(__FILE__, __LINE__, msg);        \
            if (isFatal) return;                         \
            else break;                                  \
        }                                                \
    } while(0)

#define EQ(e, a, isFatal, msg, ...)                      \
    do {                                                 \
        __typeof__(e) _e = (e);                          \
        __typeof__(a) _a = (a);                          \
        char _msg[1024];                                 \
        snprintf(_msg, sizeof(_msg), msg, ##__VA_ARGS__);\
        CHECK(_e == _a, isFatal, _msg);                  \
    } while(0)

#define ASSERT_EQ(e,a) EQ(e, a, true, "Expected %d == %d", e, a)
#define EXPECT_EQ(e,a) EQ(e, a, false, "Expected %d == %d", e, a)

#define CHECK_NULL(p, operand, isFatal)                  \
    do {                                                 \
        CHECK((p) operand NULL, isFatal, "Expected " #p " " #operand " NULL"); \
    } while(0)

#define ASSERT_NULL(p) CHECK_NULL(p, ==, true)
#define EXPECT_NULL(p) CHECK_NULL(p, ==, false)
#define ASSERT_NOT_NULL(p) CHECK_NULL(p, !=, true)
#define EXPECT_NOT_NULL(p) CHECK_NULL(p, !=, false)

#define ASSERT_FAIL(msg) CHECK(false, true, msg)

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

#define EXPECT_TRUE(expr) CHECK(expr, false, "Expected " #expr " to be true")
#define ASSERT_TRUE(expr) CHECK(expr, true, "Expected " #expr " to be true")
#define EXPECT_FALSE(expr) CHECK(!expr, false, "Expected " #expr " to be false")
#define ASSERT_FALSE(expr) CHECK(!expr, true, "Expected " #expr " to be false")

#define ASSERT_MORE_THAN(a, b) CHECK((a) > (b), true, "Expected " #a " > " #b)
#define EXPECT_MORE_THAN(a, b) CHECK((a) > (b), false, "Expected " #a " > " #b)
#define ASSERT_LESS_THAN(a, b) CHECK((a) < (b), true, "Expected " #a " < " #b)
#define EXPECT_LESS_THAN(a, b) CHECK((a) < (b), false, "Expected " #a " < " #b)

#endif
