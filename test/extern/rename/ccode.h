#include <stdio.h>

int64_t type = 42;

typedef struct _S {
  int okname;
  int type;
} S;

static inline void printS(S* myS) {
  printf("{ %d %d}\n", myS->okname, myS->type);
}
