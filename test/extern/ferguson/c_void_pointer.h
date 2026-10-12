#include <stdint.h>
static void* mytest(void* x) {
  unsigned char* xp = (unsigned char*) x;
  xp = (unsigned char*)((intptr_t)xp + 2);
  return xp;
}

