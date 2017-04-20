#include <stdio.h>
#include <stdint.h>

static void voidNoArray(int64_t n) {
  printf("%lld\n", n);
}

static void voidWithArray(int64_t* A, int64_t n) {
  printf("%lld %lld %lld\n", A[0], A[1], n);
}
