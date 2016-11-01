#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

static void bar(void (*f)(void)) {
  f();
}

static void barIntInt(int64_t (*f)(int64_t)) {
  int64_t res = f(42);
  printf("C callsite got %" PRId64 "\n", res);
}
