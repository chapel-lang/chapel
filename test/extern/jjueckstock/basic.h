#include <stdio.h>

int my_int = 42; 

static inline long add_one(long x) {
  return x+1;
}

static inline const char* greeting(void) {
  return "Hello from C land";
}

