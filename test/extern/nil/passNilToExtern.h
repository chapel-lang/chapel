#include <stdint.h>

static void foo(size_t* ptr) {
  if (ptr == NULL) {
    printf("ptr is NULL\n");
  } else {
    printf("ptr is %p\n", ptr);
  }
}
