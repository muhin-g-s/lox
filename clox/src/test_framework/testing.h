#ifndef TESTING_H
#define TESTING_H

typedef void (*test_func_t)(void);

typedef void (*setup_teardown_func_t)(void);

typedef struct test_case {
    const char *name;
    test_func_t func;
    setup_teardown_func_t setup;
    setup_teardown_func_t teardown;
} test_case_t;

typedef struct test_suite {
    const char *name;
    test_case_t *tests[128];
    int test_count;
    setup_teardown_func_t setup;
    setup_teardown_func_t teardown;
} test_suite_t;

extern test_suite_t *g_suites[];
extern int g_suite_count;

void register_suite(test_suite_t *suite);

typedef void (*suite_runner_t)(const test_suite_t *suite);

void runTestWithSetupAndTeardown(const test_case_t *test);
void runSuiteWithSetupAndTeardown(const test_suite_t *suite, const suite_runner_t runner);

const test_case_t* findTestByName(test_suite_t **suites, const char *suiteName, const char *testName);
const test_suite_t* findSuiteByName(test_suite_t **suites, const char *suiteName);

#define TEST_SUITE(suite_name, ...)                            \
    test_suite_t suite_name##_suite = { #suite_name, {0}, 0, ##__VA_ARGS__ };  \
    static void __register_suite_##suite_name(void)             \
        __attribute__((constructor));                           \
    static void __register_suite_##suite_name(void) {           \
        register_suite(&suite_name##_suite);                    \
    }

#define TEST(suite_name, test_name, ...)                         \
    static void suite_name##_##test_name(void);                  \
    static test_case_t __tc_##suite_name##_##test_name = {       \
        #test_name,                                              \
        suite_name##_##test_name,                                \
        ##__VA_ARGS__                                            \
    };                                                           \
    static void __register_##suite_name##_##test_name(void)      \
        __attribute__((constructor));                            \
    static void __register_##suite_name##_##test_name(void) {    \
        suite_name##_suite.tests[suite_name##_suite.test_count++] = &__tc_##suite_name##_##test_name; \
    }                                                            \
    static void suite_name##_##test_name(void)

#endif
