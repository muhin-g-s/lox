#include "assert.h"

int g_tests_failed = 0;

inline void assert_fail(const char* file, int line, const char* msg) {
    g_tests_failed++;
    printf("FAIL %s:%d: %s\n", file, line, msg);
}
