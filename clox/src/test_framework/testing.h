#ifndef TESTING_H
#define TESTING_H

typedef void (*test_func_t)(void);

typedef struct test_case {
    const char *name;
    test_func_t func;
} test_case_t;

typedef struct test_suite {
    const char *name;
    test_case_t *tests[128];
    int test_count;
} test_suite_t;

extern test_suite_t *g_suites[];
extern int g_suite_count;

void register_suite(test_suite_t *suite);

#define TEST_SUITE(suite_name)                                  \
    test_suite_t suite_name##_suite = { #suite_name, {0}, 0 };  \
    static void __register_suite_##suite_name(void)             \
        __attribute__((constructor));                           \
    static void __register_suite_##suite_name(void) {           \
        register_suite(&suite_name##_suite);                    \
    }

#define TEST(suite_name, test_name)                              \
    static void suite_name##_##test_name(void);                  \
    static test_case_t __tc_##suite_name##_##test_name = {       \
        #test_name, suite_name##_##test_name                     \
    };                                                           \
    static void __register_##suite_name##_##test_name(void)      \
        __attribute__((constructor));                            \
    static void __register_##suite_name##_##test_name(void) {    \
        suite_name##_suite.tests[suite_name##_suite.test_count++] = &__tc_##suite_name##_##test_name; \
    }                                                            \
    static void suite_name##_##test_name(void)

#endif
