#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <myth/myth.h>

int main(int argc, char ** argv) {
  size_t ne = (argc > 1 ? atol(argv[1]) : 7);
  size_t sz = (argc > 2 ? atol(argv[2]) : 35);
  char * a = (char *)calloc(ne, sz);
  size_t i;
  for (i = 0; i < ne * sz; i++) a[i] = 1;
  printf("OK\n");
  return 0;
}

