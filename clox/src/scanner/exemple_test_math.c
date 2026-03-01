#include "../test_framework/assert.h"
#include "../test_framework/testing.h"

int add(int a,int b){return a+b;}

TEST_SUITE(TestAdd)

TEST(TestAdd, test_add) {
    ASSERT_EQ_INT(7, add(3,4));
}

TEST_SUITE(Math)

TEST(Math, test_add_positive) {
    ASSERT_EQ_INT(5, 2+3);
}

TEST(Math, test_add_negative) {
    ASSERT_EQ_INT(-1, 2+(-3));
}
