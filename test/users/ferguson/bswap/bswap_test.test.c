
#include "bswap.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

static
void check(void* b, int len, char* match)
{
  char* buf = (char*) b;
  int i;
  int v;
  int m;
  for( i = 0; i < len; i++ ) {
    v = buf[i];
    v &= 0xff;
    m = match[i];
    m &= 0xff;
    printf("%02x ", v);
    assert(v == m);
  }
  printf("\n");
}

static
int bswap_test(int n)
{
  uint16_t a;
  uint32_t b;
  uint64_t c;

  a = n;
  a = htobe16(a);
  check(&a, sizeof(a), "\x00\x01"); // print big-endian
  a = be16toh(a);
  assert(a == n);

  a = n;
  a = htole16(a);
  check(&a, sizeof(a), "\x01\x00"); // print little-endian
  a = le16toh(a);
  assert(a == n);
 
  b = n;
  b = htobe32(n);
  check(&b, sizeof(b), "\x00\x00\x00\x01"); // print big-endian
  b = be32toh(b);
  assert(b == n);

  b = n;
  b = htole32(n);
  check(&b, sizeof(b), "\x01\x00\x00\x00"); // print little-endian
  b = le32toh(b);
  assert(b == n);
 
  c = n;
  c = htobe64(n);
  check(&c, sizeof(c), "\x00\x00\x00\x00\x00\x00\x00\x01"); // print big-endian
  c = be64toh(c);
  assert(c == n);

  c = htole64(n);
  check(&c, sizeof(c), "\x01\x00\x00\x00\x00\x00\x00\x00"); // print little-endian
  c = le64toh(c);
  assert(c == n);

  printf("bswap_test PASS\n");

  return 0;
}

int main(int argc, char** argv)
{
  // Pass in argc (which should be 1) just to
  // prevent optimization from removing the tests from runtime.
  return bswap_test(argc);
}
