#ifndef TEST_MOCKS_MEMORY_H
#define TEST_MOCKS_MEMORY_H

#include <stddef.h>

extern int g_mock_free_called;
extern int g_mock_realloc_called;
extern void* mock_realloc_result;

void free(void* ptr);
void* realloc(void* ptr, size_t size);

void reset_memory_mocks(void);

#endif