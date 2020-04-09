#include <stdio.h>
#include <stdint.h>
#include "cint.h"

ext_c_int_t my_extern_c_int = 43;
int32_t my_extern_c_int32 = 67;

void set_c_int(ext_c_int_t* lhs, int32_t rhs) {
  *lhs = (int)rhs;
}

void print_c_int(int x) {
  printf("x is %d\n", x);
}
