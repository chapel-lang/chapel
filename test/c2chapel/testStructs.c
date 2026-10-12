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

void printForwardDeclStruct1(const forwardDeclStruct1* s) {
  printf("%d\n", s->x);
}

void printForwardDeclStruct2(const struct forwardDeclStruct2* s) {
  printf("%d\n", s->x);
}

void nestedStruct_init(nestedStruct* n, int x, int y, int z) {
  n->x = x;
  n->y = y;
  n->z = z;
}

void nestedStruct_print(nestedStruct n) {
  printf("nestedStruct contents:\n");
  printf("x = %d\n", n.x);
  printf("y = %d\n", n.y);
  printf("z = %d\n", n.z);
}
