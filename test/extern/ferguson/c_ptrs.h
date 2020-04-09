#include <stdio.h>

static int* returnit(int* x) {
  return x;
}

static void printit(int* x) {
  printf("printme(%i)\n", *x);
}

static void printarr(int* x, int n) {
  int i;
  printf("printme[0..%i] = ", n-1);
  for( i = 0; i < n; i++ ) {
    printf("%i ", x[i]);
  }
  printf("\n");
}


static int* getit(void) {
  static int static_value = 22; 
  return &static_value;
}


