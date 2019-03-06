#include <stdlib.h>

#include "array_provider_real.h"

double* getExternArray(int64_t size) {
  double* res = (double*)calloc(size, sizeof(double));
  return res;
}

void freeArr(double* arr) {
  free(arr);
}
