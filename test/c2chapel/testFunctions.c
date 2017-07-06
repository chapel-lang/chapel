#include "testFunctions.h"

void test_ints(int64_t a, int64_t b) {
  printf("In 'test_ints' function\n");
  printf("First arg  = %" PRId64 "\n", a);
  printf("Second arg = %" PRId64 "\n", b);
  printf("\n");
}

void test_unsigned(uint64_t a, uint64_t b) {
  printf("In 'test_ints' function\n");
  printf("First arg  = %" PRIu64 "\n", a);
  printf("Second arg = %" PRIu64 "\n", b);
  printf("\n");
}

void test_strings(const char* str) {
  printf("In 'test_strings' function\n");
  printf("Arg = %s\n", str);
  printf("\n");
}

void test_pointer(int64_t* a) {
  *a = 100;
}

void test_array(int64_t* arr, int64_t len) {
  printf("In 'test_array' function\n");
  for (int64_t i = 0; i < len; i++) {
    printf("%" PRId64 " ", arr[i]);
  }
  printf("\n\n");
}

int64_t test_return(int64_t a) {
  return a + 1;
}
