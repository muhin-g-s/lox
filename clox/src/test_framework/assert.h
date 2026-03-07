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

#define ASSERT_EQ_INT(e,a) EQ(e, a, true, "Expected %d == %d", e, a)
#define EXPECT_EQ_INT(e,a) EQ(e, a, false, "Expected %d == %d", e, a)

#define ASSERT_EQ_UINT(e,a) EQ(e, a, true, "Expected %u == %u", e, a)
#define EXPECT_EQ_UINT(e,a) EQ(e, a, false, "Expected %u == %u", e, a)

#define ASSERT_EQ_LONG(e,a) EQ(e, a, true, "Expected %ld == %ld", e, a)
#define EXPECT_EQ_LONG(e,a) EQ(e, a, false, "Expected %ld == %ld", e, a)

#define ASSERT_EQ_ULONG(e,a) EQ(e, a, true, "Expected %lu == %lu", e, a)
#define EXPECT_EQ_ULONG(e,a) EQ(e, a, false, "Expected %lu == %lu", e, a)

#define ASSERT_EQ_LLONG(e,a) EQ(e, a, true, "Expected %lld == %lld", e, a)
#define EXPECT_EQ_LLONG(e,a) EQ(e, a, false, "Expected %lld == %lld", e, a)

#define ASSERT_EQ_ULLONG(e,a) EQ(e, a, true, "Expected %llu == %llu", e, a)
#define EXPECT_EQ_ULLONG(e,a) EQ(e, a, false, "Expected %llu == %llu", e, a)

#define ASSERT_EQ_SHORT(e,a) EQ(e, a, true, "Expected %hd == %hd", e, a)
#define EXPECT_EQ_SHORT(e,a) EQ(e, a, false, "Expected %hd == %hd", e, a)

#define ASSERT_EQ_CHAR(e,a) EQ(e, a, true, "Expected %c == %c", e, a)
#define EXPECT_EQ_CHAR(e,a) EQ(e, a, false, "Expected %c == %c", e, a)

#define ASSERT_EQ_BYTE(e,a) EQ(e, a, true, "Expected %d == %d", (int)e, (int)a)
#define EXPECT_EQ_BYTE(e,a) EQ(e, a, false, "Expected %d == %d", (int)e, (int)a)

#define ASSERT_EQ_FLOAT(e,a) EQ(e, a, true, "Expected %f == %f", e, a)
#define EXPECT_EQ_FLOAT(e,a) EQ(e, a, false, "Expected %f == %f", e, a)

#define ASSERT_EQ_DOUBLE(e,a) EQ(e, a, true, "Expected %f == %f", e, a)
#define EXPECT_EQ_DOUBLE(e,a) EQ(e, a, false, "Expected %f == %f", e, a)

#define ASSERT_EQ_FLOAT_EPS(e,a,eps) EQ(fabs((e)-(a))<=(eps), true, true, "Expected %f ≈ %f (eps=%f)", e, a, eps)
#define EXPECT_EQ_FLOAT_EPS(e,a,eps) EQ(fabs((e)-(a))<=(eps), true, false, "Expected %f ≈ %f (eps=%f)", e, a, eps)

#define ASSERT_EQ_DOUBLE_EPS(e,a,eps) EQ(fabs((e)-(a))<=(eps), true, true, "Expected %f ≈ %f (eps=%f)", e, a, eps)
#define EXPECT_EQ_DOUBLE_EPS(e,a,eps) EQ(fabs((e)-(a))<=(eps), true, false, "Expected %f ≈ %f (eps=%f)", e, a, eps)

#define ASSERT_EQ_PTR(e,a) EQ(e, a, true, "Expected %p == %p", e, a)
#define EXPECT_EQ_PTR(e,a) EQ(e, a, false, "Expected %p == %p", e, a)

#define ASSERT_EQ_STR(e,a) EQ(strcmp(e,a)==0, true, true, "Expected \"%s\" == \"%s\"", e, a)
#define EXPECT_EQ_STR(e,a) EQ(strcmp(e,a)==0, true, false, "Expected \"%s\" == \"%s\"", e, a)

#define ASSERT_EQ_STRN(e,a,n) EQ(strncmp(e,a,n)==0, true, true, "Expected \"%s\" == \"%s\" (first %d chars)", e, a, n)
#define EXPECT_EQ_STRN(e,a,n) EQ(strncmp(e,a,n)==0, true, false, "Expected \"%s\" == \"%s\" (first %d chars)", e, a, n)

#define ASSERT_EQ_STR_SAFE(e,a) EQ((e==NULL && a==NULL) || (e!=NULL && a!=NULL && strcmp(e,a)==0), true, true, "Expected \"%s\" == \"%s\"", e?e:"NULL", a?a:"NULL")
#define EXPECT_EQ_STR_SAFE(e,a) EQ((e==NULL && a==NULL) || (e!=NULL && a!=NULL && strcmp(e,a)==0), true, false, "Expected \"%s\" == \"%s\"", e?e:"NULL", a?a:"NULL")

#define ASSERT_EQ_BOOL(e,a) EQ(e, a, true, "Expected %s == %s", e?"true":"false", a?"true":"false")
#define EXPECT_EQ_BOOL(e,a) EQ(e, a, false, "Expected %s == %s", e?"true":"false", a?"true":"false")

#define ASSERT_EQ_SIZE(e,a) EQ(e, a, true, "Expected %zu == %zu", e, a)
#define EXPECT_EQ_SIZE(e,a) EQ(e, a, false, "Expected %zu == %zu", e, a)

#define ASSERT_EQ_ENUM(e,a) EQ(e, a, true, "Expected %d == %d", (int)e, (int)a)
#define EXPECT_EQ_ENUM(e,a) EQ(e, a, false, "Expected %d == %d", (int)e, (int)a)

#define ASSERT_EQ_MEM(e,a,size) EQ(memcmp(e,a,size)==0, true, true, "Expected memory blocks equal (size=%zu)", size)
#define EXPECT_EQ_MEM(e,a,size) EQ(memcmp(e,a,size)==0, true, false, "Expected memory blocks equal (size=%zu)", size)

#define ASSERT_EQ_INT16(e,a) EQ(e, a, true, "Expected %hd == %hd", e, a)
#define EXPECT_EQ_INT16(e,a) EQ(e, a, false, "Expected %hd == %hd", e, a)

#define ASSERT_EQ_UINT16(e,a) EQ(e, a, true, "Expected %hu == %hu", e, a)
#define EXPECT_EQ_UINT16(e,a) EQ(e, a, false, "Expected %hu == %hu", e, a)

#define ASSERT_EQ_INT8(e,a) EQ(e, a, true, "Expected %d == %d", (int)e, (int)a)
#define EXPECT_EQ_INT8(e,a) EQ(e, a, false, "Expected %d == %d", (int)e, (int)a)

#define ASSERT_EQ_UINT8(e,a) EQ(e, a, true, "Expected %u == %u", (unsigned int)e, (unsigned int)a)
#define EXPECT_EQ_UINT8(e,a) EQ(e, a, false, "Expected %u == %u", (unsigned int)e, (unsigned int)a)

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
