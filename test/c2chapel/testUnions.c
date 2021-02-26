#include "testUnions.h"

void intUnion_print(intUnion x) {
  printf("intUnion contents:\n");
  printf("a = %" PRId64 "\n", x.a);
  printf("b = %" PRId64 "\n", x.b);
  printf("c = %" PRId64 "\n", x.c);
  printf("\n");
}

void stringUnion_print(stringUnion y) {
  printf("stringUnion contents:\n");
  printf("a = %s\n", y.a);
  printf("b = %s\n", y.b);
  printf("\n");
}

void fnUnion_call(fnUnion f) {
  printf("Calling fnUnion.fn(42):\n");
  f.fn(42);
}
