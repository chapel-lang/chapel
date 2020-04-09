#include "stdint.h"

//typedef void* data_set;

typedef struct __data_set {
  int x;
} data_set;

int64_t read_stuff(data_set* location, void* things, uint64_t start, uint64_t size, uint64_t count);
