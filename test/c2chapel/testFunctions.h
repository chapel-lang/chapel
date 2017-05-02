#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

void test_ints(int64_t a, int64_t b);

void test_unsigned(uint64_t a, uint64_t b);

void test_strings(const char* str);

void test_pointer(int64_t* a);

void test_array(int64_t* arr, int64_t len);

int64_t test_return(int64_t a);
