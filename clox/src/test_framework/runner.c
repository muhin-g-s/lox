#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "memory.h"

#include "runner.h"
#include "testing.h"

int g_tests_run = 0;

typedef struct {
    int capacity;
    int count;
    const char **values;
} RunnedSuites;

RunnedSuites g_runned_suites = {0, 0, NULL};
    
static void initRunnedSuites(void) {
    const char** allocated = (const char**)malloc(sizeof(const char*) * g_suite_count);
    if (allocated == NULL) exit(1);
    
    g_runned_suites.values = allocated;
    g_runned_suites.capacity = g_suite_count;
    g_runned_suites.count = 0;
}

static void freeRunnedSuites(void) {
    free(g_runned_suites.values);
    g_runned_suites.capacity = 0;
    g_runned_suites.count = 0;
}

static void appendSuite(const char *suite) {
    if(g_runned_suites.count >= g_runned_suites.capacity) return;

    g_runned_suites.values[g_runned_suites.count++] = suite;
}

static void printRunnedSuites(void) {
    for(int i=0;i<g_runned_suites.count;i++) {
        printf("=== SUITE: %s ===\n", g_runned_suites.values[i]);
    }
}

static bool isFilterEmpty(const char *filter) {
    return filter == NULL || filter[0] == '\0';
}

static bool matchSuite(const char *suite, const char *filter) {
    return isFilterEmpty(filter) || strcmp(suite,filter)==0;
}

static bool matchTest(const char *test, const char *filter) {
    if(isFilterEmpty(filter)) return true;

    char *pos = strchr(filter, ':');
    if(pos == NULL) return false;

    return strcmp(test, pos+1) == 0;
}

static void runTests(const char *filter) {
    for(int i=0;i<g_suite_count;i++) {
        test_suite_t *suite = g_suites[i];

        bool suiteMatches = matchSuite(suite->name, filter);
        if(suiteMatches) {
            appendSuite(suite->name);
        };

        for(int j=0;j<suite->test_count;j++) {
            test_case_t *tc = suite->tests[j];
            if(!suiteMatches && !matchTest(tc->name, filter)) continue;
            g_tests_run++;
            tc->func();
        }
    }
}

static void printStatistics(void) {
    printRunnedSuites();
    printf("===== SUMMARY =====\n");
    printf("Total tests: %d\n", g_tests_run);
    printf("Failures   : %d\n", g_tests_failed);
}

void run(const char *filter) {
    initRunnedSuites();
    runTests(filter);
    printStatistics();
    freeRunnedSuites();
}
