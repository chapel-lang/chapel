#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <myth/myth.h>

int main(int argc, char ** argv) {
  size_t sz = (argc > 1 ? atol(argv[1]) : 35);
  char * a = (char *)malloc(sz);
  size_t i;
  for (i = 0; i < sz; i++) a[i] = 1;
  printf("OK\n");
  return 0;
}

