#include <stdio.h>
#include <stdint.h>
#include "cint.h"

c_int my_extern_c_int = 43;

void set_c_int(c_int* lhs, int32_t rhs) {
  *lhs = (int)rhs;
}

void print_c_int(int x) {
  printf("x is %d\n", x);
}
