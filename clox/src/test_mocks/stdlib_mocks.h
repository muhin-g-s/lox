#ifndef TEST_MOCKS_STDLIB_H
#define TEST_MOCKS_STDLIB_H

#include <stdbool.h>
#include <setjmp.h>

extern int g_mock_exit_called;
extern int mock_exit_code;

void exit(int code);
bool is_first_call(void);
void reset_stdlib_mocks(void);

#endif