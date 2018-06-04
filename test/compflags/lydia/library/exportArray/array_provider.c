#include <stdlib.h>

#include "array_provider.h"

int64_t* getExternArray(int64_t size) {
  int64_t* res = (int64_t*)calloc(size, sizeof(int64_t));
  return res;
}

void freeArr(int64_t* arr) {
  free(arr);
}
