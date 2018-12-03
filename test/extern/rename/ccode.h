#include <stdio.h>

int64_t type = 42;

typedef struct _S {
  int64_t okname;
  int64_t type;
} S;

static inline void printS(S* myS) {
  printf("{ %lld %lld}\n", myS->okname, myS->type);
}
