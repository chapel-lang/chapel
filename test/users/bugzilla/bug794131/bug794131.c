#include "bug794131.h"
#include <string.h>

// extern proc read_stuff(location: c_ptr(data_set), stuff: [] things,
//                        start: uint(64), size: uint(64),
//                        count: uint(64)): int(64);

int64_t read_stuff(data_set* location, void* things, uint64_t start, uint64_t size, uint64_t count) {
  memset(((uint64_t*)things)+start, 0x2A, size*count);
  return count;
}



