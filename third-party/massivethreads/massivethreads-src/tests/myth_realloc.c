#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <myth/myth.h>

int main(int argc, char ** argv) {
  size_t sz0 = (argc > 1 ? atol(argv[1]) : 35);
  size_t sz1 = (argc > 1 ? atol(argv[1]) : 35);
  char * a = (char *)malloc(sz0);
  size_t i;
  for (i = 0; i < sz0; i++) a[i] = 1;
  char * b = realloc(a, sz1);
  for (i = 0; i < sz1; i++) b[i] = 1;
  printf("OK\n");
  return 0;
}

