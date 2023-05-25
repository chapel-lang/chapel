#include <stdio.h>
#include <stdlib.h>
#include "link2.h"

void foo(const char* s) {
  printf("in c routine foo, printing %s\n", s);
}

char* bar(int i) {
  char* s;
  size_t slen = 50;
  s = (char*)malloc(slen);
  snprintf(s, slen, "sprintf %d", i);
  return s;
}
