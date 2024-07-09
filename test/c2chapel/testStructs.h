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

struct forwardDeclStruct1;

typedef struct forwardDeclStruct1 { int x; } forwardDeclStruct1;

void printForwardDeclStruct1(const forwardDeclStruct1*);

struct forwardDeclStruct2;

struct forwardDeclStruct2 { int x; };

void printForwardDeclStruct2(const struct forwardDeclStruct2*);
