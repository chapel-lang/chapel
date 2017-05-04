#include "testStructs.h"

void intStruct_print(intStruct x) {
  printf("intStruct contents:\n");
  printf("a = %" PRId64 "\n", x.a);
  printf("b = %" PRId64 "\n", x.b);
  printf("c = %" PRId64 "\n", x.c);
  printf("\n");
}

void stringStruct_print(stringStruct y) {
  printf("stringStruct contents:\n");
  printf("a = %s\n", y.a);
  printf("b = %s\n", y.b);
  printf("\n");
}

void fnStruct_call(fnStruct f) {
  printf("Calling fnStruct.fn(42):\n");
  f.fn(42);
}
