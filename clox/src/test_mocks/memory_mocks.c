#include "memory_mocks.h"

int g_mock_free_called = 0;
int g_mock_realloc_called = 0;
void* mock_realloc_result = NULL;

void free(void* ptr) {
    g_mock_free_called++;
}

void* realloc(void* ptr, size_t size) {
    g_mock_realloc_called++;
    return mock_realloc_result;
}

void reset_memory_mocks(void* realloc_result) {
    g_mock_free_called = 0;
    g_mock_realloc_called = 0;
    mock_realloc_result = realloc_result;
}