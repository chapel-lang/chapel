#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct {
  int64_t a;
  int64_t b;
  int64_t c;
} intStruct;

void intStruct_print(intStruct x);

typedef struct {
  const char* a;
  const char* b;
} stringStruct;

void stringStruct_print(stringStruct y);

typedef struct {
  void (*fn)(int64_t);
} fnStruct;

void fnStruct_call(fnStruct f);
