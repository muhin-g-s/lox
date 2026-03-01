#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>

extern int g_tests_failed;

#define ASSERT_EQ_INT(e,a)                               \
    do {                                                 \
        int _e = (e);                                    \
        int _a = (a);                                    \
        if (_e != _a) {                                  \
            g_tests_failed++;                            \
            printf("FAIL %s:%d: expected %d, got %d\n", __FILE__, __LINE__, _e, _a); \
            return;                                      \
        }                                                \
    } while(0)

#endif
