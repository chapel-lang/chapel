#include <stdio.h>

static void printarr(const int* x, int n) {
  int i;
  printf("printme[0..%i] = ", n-1);
  for( i = 0; i < n; i++ ) {
    printf("%i ", x[i]);
  }
  printf("\n");
}

static int sumarr(const int* x, int n) {
  int i;
  int acc = 0;
  printf("sumarr[0..%i] = ", n-1);
  for( i = 0; i < n; i++ ) {
    acc += x[i];
  }
  printf("%i\n", acc);
  return acc;
}
