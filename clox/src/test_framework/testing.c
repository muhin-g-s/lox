#include <string.h>
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

const test_case_t* findTestByName(test_suite_t **suites, const char *suiteName, const char *testName) {
    for(int i=0;i<g_suite_count;i++) {
        test_suite_t *suite = suites[i];
        for(int j=0;j<suite->test_count;j++) {
            test_case_t *test = suite->tests[j];
            if(strcmp(suite->name, suiteName) == 0 && strcmp(test->name, testName) == 0) {
                return test;
            }
        }
    }

    return NULL;
}

const test_suite_t* findSuiteByName(test_suite_t **suites, const char *suiteName) {
    for(int i=0;i<g_suite_count;i++) {
        test_suite_t *suite = suites[i];
        if(strcmp(suite->name, suiteName) == 0) {
            return suite;
        }
    }

    return NULL;
}

void runTestWithSetupAndTeardown(const test_case_t *test) {
    if (test->setup) test->setup();
    test->func();
    if (test->teardown) test->teardown();
}

void runSuiteWithSetupAndTeardown(const test_suite_t *suite, const suite_runner_t runner) {
    if (suite->setup) suite->setup();
    runner(suite);
    if (suite->teardown) suite->teardown();
}
