#ifndef _SUN_STDINT_H_
#define _SUN_STDINT_H_

#include <stdlib.h>

typedef int intptr_t;
typedef unsigned int uintptr_t;

static unsigned long long strtoull(const char* str, char** endptr, int base) {
  return strtoul(str, endptr, base);
}

static long long strtoll(const char* str, char** endptr, int base) {
  return strtol(str, endptr, base);
}


#endif

