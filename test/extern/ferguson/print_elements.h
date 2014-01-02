#include <stdio.h>

static void printit(int* x, int n) {
  int i;
  for( i = 0; i < n; i++ ) {
    printf("%i ", x[i]);
  }
  printf("\n");
}

