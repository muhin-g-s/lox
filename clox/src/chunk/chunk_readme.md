```c
#include "chunk.h"
#include "value.h"

void buildExampleChunk(Chunk* chunk) {
    initChunk(chunk);

    int constant1 = addConstant(chunk, NUMBER_VAL(1.2));
    writeChunk(chunk, OP_CONSTANT, 1);
    writeChunk(chunk, constant1, 1);

    int constant2 = addConstant(chunk, NUMBER_VAL(3.4));
    writeChunk(chunk, OP_CONSTANT, 1);
    writeChunk(chunk, constant2, 1);

    writeChunk(chunk, OP_ADD, 1);
    writeChunk(chunk, OP_NEGATE, 1);
    writeChunk(chunk, OP_RETURN, 1);
}
```