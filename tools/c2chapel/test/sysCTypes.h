#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

// TODO: Have c2chapel handle standard headers cleanly
// Handle int64_t, etc

int test_int(int a, int* b);

unsigned int test_uint(unsigned int a, int b, unsigned int* c);

long test_longs(long a, unsigned long b, long long c, unsigned long long d,
                long long* e);

char test_chars(char a, char* b, signed char c, unsigned char d);

short test_shorts(short a, unsigned short b);

void test_ptrs(intptr_t a, uintptr_t b, ptrdiff_t c, ssize_t d, size_t e);

void signedWidths(int8_t a, int16_t b, int32_t c, int64_t d);
void unsignedWidths(uint8_t a, uint16_t b, uint32_t c, uint64_t d);
