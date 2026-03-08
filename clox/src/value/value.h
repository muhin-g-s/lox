#ifndef clox_value_h
#define clox_value_h

#include "../common/common.h"

typedef double Value;

typedef struct {
  int capacity;
  int count;
  Value* values;
} ValueArray;

#define CREATE_VALUE(val) val 
bool valuesEqual(Value a, Value b);

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
bool valuesArrayEquels(ValueArray a, ValueArray b);
void printValue(Value value);

#endif
