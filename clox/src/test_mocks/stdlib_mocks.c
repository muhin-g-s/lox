#include "stdlib_mocks.h"

int g_mock_exit_called = 0;
int mock_exit_code = 0;
jmp_buf mock_exit_env;

void exit(int code) {
    g_mock_exit_called = 1;
    mock_exit_code = code;
    longjmp(mock_exit_env, 1);
}

void reset_stdlib_mocks(void) {
    g_mock_exit_called = 0;
    mock_exit_code = 0;
}