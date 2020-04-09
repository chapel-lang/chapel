#include <stdio.h>
#include <stdlib.h>

#include "struct_with_fn_ptr.h"

MyRec getMyRec() {
  MyRec ret;
  ret.x = free;
  return ret;
}

void takeFuncPtr(func* arg) {
  printf("yay\n");
}
