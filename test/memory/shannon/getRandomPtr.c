#include "getRandomPtr.h"

long cCantFreeMe = 13;

void* getRandomPtr() {
  return (void*)&cCantFreeMe;
}

