#include "memory_mocks.h"

int g_mock_free_called = 0;
int g_mock_realloc_called = 0;

static void** memmory = NULL;
static int countAllocs = 0;
static int maxAllocs = 0;

void free(void* ptr) {
    g_mock_free_called++;
}

void* realloc(void* ptr, size_t size) {
    g_mock_realloc_called++;

    if (countAllocs < maxAllocs && memmory != NULL) {
        return memmory[countAllocs++];
    }

    return NULL;
}

void setupMemoryMocks() {
    g_mock_free_called = 0;
    g_mock_realloc_called = 0;

    memmory = NULL;
    countAllocs = 0;
    maxAllocs = 0;
}

void setupMemoryMocksWithMemory(void** bloks, int max_allocs) {
    setupMemoryMocks();
    memmory = bloks;
    maxAllocs = max_allocs;
}
