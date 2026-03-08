#include <stdlib.h>

#include "chunk.h"
#include "../memory/memory.h"

void initChunk(Chunk* chunk) {
  chunk->countCode = 0;
  chunk->capacityCode = 0;
  chunk->code = NULL;

  chunk->countLines = 0;
  chunk->capacityLines = 0;
	chunk->lines = NULL;

	initValueArray(&chunk->constants);
}

// Отрефакторить
void writeChunk(Chunk* chunk, uint8_t byte, int line) {
  if (chunk->capacityCode < chunk->countCode + 1) {
    int oldCapacity = chunk->capacityCode;
    chunk->capacityCode = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacityCode);
  }

  chunk->code[chunk->countCode] = byte;
  chunk->countCode++;

  if(line > chunk->countLines) {
    int oldCapacity = chunk->capacityLines;
    
    if (chunk->capacityLines < line) {
      chunk->capacityLines = GROW_CAPACITY(oldCapacity);
      chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacityLines);
    }

    chunk->countLines = line;
  }

  chunk->lines[line - 1] = chunk->countCode - 1;
}

void freeChunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacityCode);
	FREE_ARRAY(int, chunk->lines, chunk->capacityCode);

	freeValueArray(&chunk->constants);

  initChunk(chunk);
}

int addConstant(Chunk* chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}

// Проверить мб стоит где-то еще юзать 
Value getConstantByIndex(Chunk* chunk, int index) {
  return chunk->constants.values[index];
}

// Проверить мб стоит где-то еще юзать 
uint8_t getCodeByIndex(Chunk* chunk, int index) {
    return chunk->code[index];
}

// Cделать через бинарный поиск
int getLine(Chunk* chunk, int instruction) {
  if(instruction < 0) {
    return -1;
  }

  for (int i = 0; i < chunk->countLines; i++) {
    if (instruction <= chunk->lines[i]) {
      return i + 1;
    }
  }

  return -1;
}