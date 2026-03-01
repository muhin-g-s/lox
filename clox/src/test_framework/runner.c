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

typedef struct {
    const char *suiteName;
    const char *testName;
} Filter;

#define SEPORATOR ':'

static Filter parseFilter(const char *filter) {
    Filter f = {0};
    const char *seporator = strchr(filter, SEPORATOR);
    if (seporator) {
        f.suiteName = strndup(filter, seporator - filter);
        f.testName = strdup(seporator + 1);
    } else {
        f.suiteName = strdup(filter);
    }
    return f;
}

static void freeFilter(Filter *f) {
    free((void*)f->suiteName);
    free((void*)f->testName);
}

static bool filterHasTestName(const Filter *f) {
    return f->testName != NULL;
}

static void runSuite(const test_suite_t *suite) {
    appendSuite(suite->name);

    for(int j=0;j<suite->test_count;j++) {
        test_case_t *tc = suite->tests[j];
        g_tests_run++;
        runTestWithSetupAndTeardown(tc);
    }
}

static void runAll(void) {
    for(int i=0;i<g_suite_count;i++) {
        test_suite_t *suite = g_suites[i];
        runSuiteWithSetupAndTeardown(suite, runSuite);
    }
}

static bool runByFilter(const Filter *filter) {
    if(filterHasTestName(filter)) {
        const test_case_t *tc = findTestByName(g_suites, filter->suiteName, filter->testName);
        if(tc) {
            runTestWithSetupAndTeardown(tc);
            return true;
        } else {
            printf("Test '%s' not found in suite '%s'\n", filter->testName, filter->suiteName);
            return false;
        }
    } else {
        const test_suite_t *suite = findSuiteByName(g_suites, filter->suiteName);
        if(suite) {
            runSuiteWithSetupAndTeardown(suite, runSuite);
            return true;
        } else {
            printf("Suite '%s' not found\n", filter->suiteName);
            return false;
        }
    }
}

static bool runTests(const char *filterRaw) {
    if (filterRaw == NULL) {
        runAll();
        return true;
    }

    Filter filter = parseFilter(filterRaw);
    bool result = runByFilter(&filter);
    freeFilter(&filter);

    return result;
}

static void printStatistics(void) {
    printRunnedSuites();
    printf("===== SUMMARY =====\n");
    printf("Total tests: %d\n", g_tests_run);
    printf("Failures   : %d\n", g_tests_failed);
}

void run(const char *filter) {
    initRunnedSuites();

    bool result = runTests(filter);
    if (!result) {
        freeRunnedSuites();
        return;
    }

    printStatistics();
    freeRunnedSuites();
}
