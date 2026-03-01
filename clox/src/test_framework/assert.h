#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>
#include <stdbool.h>

extern int g_tests_failed;

#define EQ_INT(e,a, isFatal)                             \
    do {                                                 \
        int _e = (e);                                    \
        int _a = (a);                                    \
        if (_e != _a) {                                  \
            g_tests_failed++;                            \
            printf("FAIL %s:%d: expected %d, got %d\n", __FILE__, __LINE__, _e, _a); \
            if (isFatal) return;                         \
            else break;                                  \
        }                                                \
    } while(0)

#define ASSERT_EQ_INT(e,a) EQ_INT(e,a, true)
#define EXPECT_EQ_INT(e,a) EQ_INT(e,a, false)

#endif
