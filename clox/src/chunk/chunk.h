#ifndef clox_chunk_h
#define clox_chunk_h

#include "../common/common.h"
#include "../value/value.h"

typedef enum {
  OP_RETURN,
	OP_NEGATE,
	OP_CONSTANT,
	OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
} OpCode;

typedef struct {
	int countCode;
  int capacityCode;
  uint8_t* code;

	ValueArray constants;

  int  countLines;
  int  capacityLines;
	int* lines;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);

void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);

int getLine(Chunk* chunk, int instruction);

#endif
