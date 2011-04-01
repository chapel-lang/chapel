#include <string.h>
#include "array_ops.h"

void
clear_int_array(int *A, int len) {
  memset(A, 0, len*sizeof(int));
}


void
copy_int_array(int *dest, int *src, int len) {
  memcpy(dest, src, len*sizeof(int));
}

