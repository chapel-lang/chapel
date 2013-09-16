#include <stdio.h>

static int* returnit(int* x) {
  return x;
}

static void printit(int* x) {
  printf("printme(%i)\n", *x);
}

static int* getit(void) {
  static int static_value = 22; 
  return &static_value;
}


