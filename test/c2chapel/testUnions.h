#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

typedef union {
  int64_t a;
  int64_t b;
  int64_t c;
} intUnion;

void intUnion_print(intUnion x);

typedef struct {
  const char* a;
  const char* b;
} stringUnion;

void stringUnion_print(stringUnion y);

typedef struct {
  void (*fn)(int64_t);
} fnUnion;

void fnUnion_call(fnUnion f);

