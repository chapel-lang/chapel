#include "getRandomPtr.h"

long cantFreeMe = 13;

void* getRandomPtr() {
  return (void*)&cantFreeMe;
}

