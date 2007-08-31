#include <stdio.h>
#include <stdlib.h>
#include "link1.h"

void foo(const char* s) {
  printf("in c routine foo, printing %s\n", s);
}

char* bar(int i) {
  char* s;
  s = (char*)malloc(50*sizeof(char));
  sprintf(s, "sprintf %d", i);
  return s;
}
