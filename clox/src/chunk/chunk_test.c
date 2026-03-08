#include "../test_framework/assert.h"
#include "../test_framework/testing.h"
#include "../test_mocks/memory_mocks.h"
#include "../test_mocks/stdlib_mocks.h"
#include "chunk.h"

TEST_SUITE(Chunk)

TEST(Chunk, initChunk) {
    Chunk chunk;
    ValueArray valueArray;

    initChunk(&chunk);
    initValueArray(&valueArray);

    EXPECT_EQ(0, chunk.capacityCode);
    EXPECT_EQ(0, chunk.capacityLines);
    EXPECT_EQ(0, chunk.countCode);
    EXPECT_EQ(0, chunk.countLines);

    EXPECT_NULL(chunk.code);
    EXPECT_NULL(chunk.lines);

    EXPECT_TRUE(valuesArrayEquels(valueArray, chunk.constants));
}

/*
    1 writeChunkAndGrowCapacityCode
    2 writeChunkAndNotGrowCapacityCode

    3 writeChunkSameLine
    4 writeChunkNotSameLineAndGrowCapacityLines
    5 writeChunkNotSameLineAndNotGrowCapacityLines

    capacityCode
    code
    countCode

    capacityLines
    lines
    countLines
*/


void assertChunkInvariant(Chunk* chunk) {
    // EXPECT_MORE_OR_EQ(chunk->capacityCode, chunk->countCode);
    // EXPECT_MORE_OR_EQ(chunk->capacityLines, chunk->countLines);
}

TEST(Chunk, writeChunk, setupMemoryMocks) {
    Chunk chunk;
    ValueArray valueArray;
    uint8_t byte = 0xAB;
    int line = 1;
    int instructionCount = 0;
    initChunk(&chunk);
    initValueArray(&valueArray);


    writeChunk(&chunk, byte, line);


    EXPECT_EQ(instructionCount + 1, chunk.countCode);
    EXPECT_MORE_THAN(chunk.capacityCode, 0);
    EXPECT_EQ_UINT8(byte, getCodeByIndex(&chunk, instructionCount));
    
    EXPECT_EQ(line, chunk.countLines);
    EXPECT_MORE_THAN(chunk.capacityLines, 0);
    EXPECT_EQ(line, getLine(&chunk, instructionCount));

    EXPECT_TRUE(valuesArrayEquels(valueArray, chunk.constants));
}

TEST(Chunk, writeChunkSameLine, setupMemoryMocks) {
    Chunk chunk;
    ValueArray valueArray;
    uint8_t byte = 0xAB;
    int line = 1;
    int instructionCount = 1;
    initChunk(&chunk);
    initValueArray(&valueArray);


    writeChunk(&chunk, byte, line);
    writeChunk(&chunk, byte, line);

    
    EXPECT_EQ(instructionCount + 1, chunk.countCode);
    EXPECT_MORE_THAN(chunk.capacityCode, 0);
    EXPECT_EQ_UINT8(byte, getCodeByIndex(&chunk, instructionCount));
    
    EXPECT_EQ(line, chunk.countLines);
    EXPECT_MORE_THAN(chunk.capacityLines, 0);
    EXPECT_EQ(line, getLine(&chunk, instructionCount));
    EXPECT_EQ(line, getLine(&chunk, instructionCount - 1));

    EXPECT_TRUE(valuesArrayEquels(valueArray, chunk.constants));
}

TEST(Chunk, writeSomeChunkSameLine, setupMemoryMocks) {
    Chunk chunk;
    ValueArray valueArray;
    uint8_t byte = 0x00;
    int line = 1;
    int instructionCount = 0;
    initChunk(&chunk);
    initValueArray(&valueArray);

    #define MAX_INSTRUCTIONS 100

    while(instructionCount < MAX_INSTRUCTIONS) {
        writeChunk(&chunk, byte, line);
        byte++;
        line++;
        instructionCount++;
    }

    EXPECT_EQ(10, g_mock_realloc_called);

    EXPECT_EQ(instructionCount, chunk.countCode);
    EXPECT_MORE_THAN(chunk.capacityCode, instructionCount);

    
    EXPECT_EQ(instructionCount, chunk.countLines);
    EXPECT_MORE_THAN(chunk.capacityLines, instructionCount);

    instructionCount--;
    line--;
    byte--;
    while(instructionCount > 0) {
        EXPECT_EQ_UINT8(byte, getCodeByIndex(&chunk, instructionCount));
        EXPECT_EQ(line, getLine(&chunk, instructionCount));
        instructionCount--;
        line--;
        byte--;
    }

    EXPECT_TRUE(valuesArrayEquels(valueArray, chunk.constants));
}


TEST(Chunk, freeChunk) {
    setupMemoryMocks();

    Chunk chunk;
    ValueArray valueArray;
    initValueArray(&valueArray);

    freeChunk(&chunk);

    EXPECT_EQ(3, g_mock_free_called);
    
    EXPECT_EQ(0, chunk.capacityCode);
    EXPECT_EQ(0, chunk.capacityLines);
    EXPECT_EQ(0, chunk.countCode);
    EXPECT_EQ(0, chunk.countLines);

    EXPECT_NULL(chunk.code);
    EXPECT_NULL(chunk.lines);

    EXPECT_TRUE(valuesArrayEquels(valueArray, chunk.constants));
}

TEST(Chunk, addConstant, setupMemoryMocks) {
    Chunk chunk;
    Value testValue = CREATE_VALUE(0.0); 
    initChunk(&chunk);

    int constantIndex = addConstant(&chunk, testValue);

    EXPECT_EQ(0, constantIndex);
    EXPECT_TRUE(valuesEqual(testValue, getConstantByIndex(&chunk, constantIndex)));
}

TEST(Chunk, getLineEmptyChunk) {
    Chunk chunk;

    initChunk(&chunk);

    EXPECT_EQ(-1, getLine(&chunk, 1));
}

TEST(Chunk, getLineLineNotFound, setupMemoryMocks) {
    Chunk chunk;
    uint8_t byte = 0xAB;
    int line = 1;
    initChunk(&chunk);

    writeChunk(&chunk, byte, line);

    EXPECT_EQ(-1, getLine(&chunk, 2));
}

TEST(Chunk, getLineLineFound, setupMemoryMocks) {
    Chunk chunk;
    uint8_t byte = 0xAB;
    int line = 1;
    int instructionCount = 0;
    initChunk(&chunk);

    writeChunk(&chunk, byte, line);

    EXPECT_EQ(line, getLine(&chunk, instructionCount));
}

TEST(Chunk, getLineLineNegative, setupMemoryMocks) {
    Chunk chunk;
    uint8_t byte = 0xAB;
    int line = 1;
    initChunk(&chunk);

    writeChunk(&chunk, byte, line);

    EXPECT_EQ(-1, getLine(&chunk, -5));
}