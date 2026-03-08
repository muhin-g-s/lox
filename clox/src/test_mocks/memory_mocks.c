#include "memory_mocks.h"

#include <string.h>

int g_mock_free_called = 0;
int g_mock_realloc_called = 0;

typedef struct {
    void* ptr;
    size_t size;
} MockBlock;

typedef struct {
    void* ptr;
    size_t size;
} Allocation;

#define BLOCKS 32
#define BLOCK_SIZE 1024
typedef long long buffer_t;

static buffer_t  b1[BLOCK_SIZE],  b2[BLOCK_SIZE],  b3[BLOCK_SIZE],  b4[BLOCK_SIZE],
b5[BLOCK_SIZE],  b6[BLOCK_SIZE],  b7[BLOCK_SIZE],  b8[BLOCK_SIZE],  b9[BLOCK_SIZE], 
b10[BLOCK_SIZE], b11[BLOCK_SIZE], b12[BLOCK_SIZE], b13[BLOCK_SIZE], b14[BLOCK_SIZE], 
b15[BLOCK_SIZE], b16[BLOCK_SIZE], b17[BLOCK_SIZE], b18[BLOCK_SIZE], b19[BLOCK_SIZE], 
b20[BLOCK_SIZE], b21[BLOCK_SIZE], b22[BLOCK_SIZE], b23[BLOCK_SIZE], b24[BLOCK_SIZE],
b25[BLOCK_SIZE], b26[BLOCK_SIZE], b27[BLOCK_SIZE], b28[BLOCK_SIZE], b29[BLOCK_SIZE],
b30[BLOCK_SIZE], b31[BLOCK_SIZE], b32[BLOCK_SIZE];

#define BUFFER_SIZE BLOCK_SIZE * sizeof(buffer_t)

static Allocation allocations[BLOCKS];
static MockBlock memoryBlocks[BLOCKS] = {
    {b1, BUFFER_SIZE},  {b2, BUFFER_SIZE},  {b3, BUFFER_SIZE},  {b4, BUFFER_SIZE},  {b5, BUFFER_SIZE},
    {b6, BUFFER_SIZE},  {b7, BUFFER_SIZE},  {b8, BUFFER_SIZE},  {b9, BUFFER_SIZE},  {b10, BUFFER_SIZE}, 
    {b11, BUFFER_SIZE}, {b12, BUFFER_SIZE}, {b13, BUFFER_SIZE}, {b14, BUFFER_SIZE}, {b15, BUFFER_SIZE}, 
    {b16, BUFFER_SIZE}, {b17, BUFFER_SIZE}, {b18, BUFFER_SIZE}, {b19, BUFFER_SIZE}, {b20, BUFFER_SIZE}, 
    {b21, BUFFER_SIZE}, {b22, BUFFER_SIZE}, {b23, BUFFER_SIZE}, {b24, BUFFER_SIZE}, {b25, BUFFER_SIZE}, 
    {b26, BUFFER_SIZE}, {b27, BUFFER_SIZE}, {b28, BUFFER_SIZE}, {b29, BUFFER_SIZE}, {b30, BUFFER_SIZE},
    {b31, BUFFER_SIZE},  {b32, BUFFER_SIZE}
};

static int countAllocs = 0;

void free(void* ptr) {
    g_mock_free_called++;
}

void* realloc(void* ptr, size_t size) {
    g_mock_realloc_called++;

    if(countAllocs >= BLOCKS) {
        return NULL;
    }

    size_t oldAllocSize = 0;
    void* oldAllocPtr = NULL;

    for(int i = 0; i < countAllocs && ptr != NULL; i++) {
        Allocation oldAlloc = allocations[i];
        if(oldAlloc.ptr == ptr) {
            oldAllocSize = oldAlloc.size;
            oldAllocPtr = oldAlloc.ptr;
            break;
        }
    }

    MockBlock newBlock = memoryBlocks[countAllocs];
    size_t minSize = (newBlock.size < oldAllocSize) ? newBlock.size : oldAllocSize;

    if(oldAllocPtr != NULL && oldAllocSize != 0) {
        memcpy(newBlock.ptr, oldAllocPtr, minSize);
    }

    allocations[countAllocs].ptr = newBlock.ptr;
    allocations[countAllocs].size = size;

    countAllocs++;

    return newBlock.ptr;
}

void setupMemoryMocks() {
    g_mock_free_called = 0;
    g_mock_realloc_called = 0;
    memset(allocations, 0, sizeof(allocations));
    countAllocs = 0;
}
