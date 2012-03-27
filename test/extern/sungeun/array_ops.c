#include <string.h>
#include "array_ops.h"

void
clear_int_array(int64_t *A, int len) {
  memset(A, 0, len*sizeof(int64_t));
}


void
copy_int_array(int64_t *dest, int64_t *src, int len) {
  memcpy(dest, src, len*sizeof(int64_t));
}

