#include <stdio.h>
#include <stdint.h>

static void voidNoArray(int64_t n) {
  printf("%d\n", (int)n);
}

static void voidWithArray(int64_t* A, int64_t n) {
  printf("%d %d %d\n", (int)A[0], (int)A[1], (int)n);
}
