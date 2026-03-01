#include "testing.h"

#define MAX_SUITES 128
test_suite_t *g_suites[MAX_SUITES];

int g_suite_count = 0;

void register_suite(test_suite_t *suite) {
    for(int i=0;i<g_suite_count;i++)
        if(g_suites[i]==suite) return;
    if(g_suite_count < MAX_SUITES) {
        g_suites[g_suite_count++] = suite;
    }
}
