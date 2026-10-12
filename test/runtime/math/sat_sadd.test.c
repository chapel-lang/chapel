
#include "chpl-rt-math.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

// Check a single signed saturating add for a given type.
#define CHECK_SADD(x, y, expected, stype, utype, smax) \
  do { \
    stype _res; \
    CHPL_SAT_SADD(_res, (stype)(x), (stype)(y), stype, utype, smax); \
    if (_res != (stype)(expected)) { \
      printf("SADD on %s failed: %lld + %lld = %lld, expected %lld\n", \
              #stype, (long long)(x), (long long)(y), (long long)_res, (long long)(expected)); \
    } \
  } while (0)

static void test_int8(void) {
  const int8_t MAX = INT8_MAX;   // 127
  const int8_t MIN = INT8_MIN;   // -128

  // normal operation, no overflow
  CHECK_SADD(0, 0, 0, int8_t, uint8_t, MAX);
  CHECK_SADD(1, 2, 3, int8_t, uint8_t, MAX);
  CHECK_SADD(-1, -2, -3, int8_t, uint8_t, MAX);
  CHECK_SADD(10, -4, 6, int8_t, uint8_t, MAX);
  CHECK_SADD(-10, 4, -6, int8_t, uint8_t, MAX);

  // identity with zero
  CHECK_SADD(MAX, 0, MAX, int8_t, uint8_t, MAX);
  CHECK_SADD(MIN, 0, MIN, int8_t, uint8_t, MAX);
  CHECK_SADD(0, MAX, MAX, int8_t, uint8_t, MAX);
  CHECK_SADD(0, MIN, MIN, int8_t, uint8_t, MAX);

  // opposite signs cancel
  CHECK_SADD(-1, 1, 0, int8_t, uint8_t, MAX);
  CHECK_SADD(MAX, MIN, -1, int8_t, uint8_t, MAX);

  // boundary, exactly reaches the limit without saturating
  CHECK_SADD(MAX - 1, 1, MAX, int8_t, uint8_t, MAX);
  CHECK_SADD(MIN + 1, -1, MIN, int8_t, uint8_t, MAX);

  // positive overflow saturates to MAX
  CHECK_SADD(MAX, 1, MAX, int8_t, uint8_t, MAX);
  CHECK_SADD(MAX, MAX, MAX, int8_t, uint8_t, MAX);
  CHECK_SADD(100, 100, MAX, int8_t, uint8_t, MAX);

  // negative overflow saturates to MIN
  CHECK_SADD(MIN, -1, MIN, int8_t, uint8_t, MAX);
  CHECK_SADD(MIN, MIN, MIN, int8_t, uint8_t, MAX);
  CHECK_SADD(-100, -100, MIN, int8_t, uint8_t, MAX);
}

static void test_int16(void) {
  const int16_t MAX = INT16_MAX;
  const int16_t MIN = INT16_MIN;

  CHECK_SADD(0, 0, 0, int16_t, uint16_t, MAX);
  CHECK_SADD(1234, 4321, 5555, int16_t, uint16_t, MAX);
  CHECK_SADD(-1234, -4321, -5555, int16_t, uint16_t, MAX);

  CHECK_SADD(MAX, 0, MAX, int16_t, uint16_t, MAX);
  CHECK_SADD(MIN, 0, MIN, int16_t, uint16_t, MAX);
  CHECK_SADD(-1, 1, 0, int16_t, uint16_t, MAX);
  CHECK_SADD(MAX, MIN, -1, int16_t, uint16_t, MAX);

  CHECK_SADD(MAX - 1, 1, MAX, int16_t, uint16_t, MAX);
  CHECK_SADD(MIN + 1, -1, MIN, int16_t, uint16_t, MAX);

  CHECK_SADD(MAX, 1, MAX, int16_t, uint16_t, MAX);
  CHECK_SADD(MAX, MAX, MAX, int16_t, uint16_t, MAX);
  CHECK_SADD(MIN, -1, MIN, int16_t, uint16_t, MAX);
  CHECK_SADD(MIN, MIN, MIN, int16_t, uint16_t, MAX);
}

static void test_int32(void) {
  const int32_t MAX = INT32_MAX;
  const int32_t MIN = INT32_MIN;

  CHECK_SADD(0, 0, 0, int32_t, uint32_t, MAX);
  CHECK_SADD(100000, 23456, 123456, int32_t, uint32_t, MAX);
  CHECK_SADD(-100000, -23456, -123456, int32_t, uint32_t, MAX);

  CHECK_SADD(MAX, 0, MAX, int32_t, uint32_t, MAX);
  CHECK_SADD(MIN, 0, MIN, int32_t, uint32_t, MAX);
  CHECK_SADD(-1, 1, 0, int32_t, uint32_t, MAX);
  CHECK_SADD(MAX, MIN, -1, int32_t, uint32_t, MAX);

  CHECK_SADD(MAX - 1, 1, MAX, int32_t, uint32_t, MAX);
  CHECK_SADD(MIN + 1, -1, MIN, int32_t, uint32_t, MAX);

  CHECK_SADD(MAX, 1, MAX, int32_t, uint32_t, MAX);
  CHECK_SADD(MAX, MAX, MAX, int32_t, uint32_t, MAX);
  CHECK_SADD(MIN, -1, MIN, int32_t, uint32_t, MAX);
  CHECK_SADD(MIN, MIN, MIN, int32_t, uint32_t, MAX);
}

static void test_int64(void) {
  const int64_t MAX = INT64_MAX;
  const int64_t MIN = INT64_MIN;

  CHECK_SADD(0, 0, 0, int64_t, uint64_t, MAX);
  CHECK_SADD(1000000000LL, 7, 1000000007LL, int64_t, uint64_t, MAX);
  CHECK_SADD(-1000000000LL, -7, -1000000007LL, int64_t, uint64_t, MAX);

  CHECK_SADD(MAX, 0, MAX, int64_t, uint64_t, MAX);
  CHECK_SADD(MIN, 0, MIN, int64_t, uint64_t, MAX);
  CHECK_SADD(-1, 1, 0, int64_t, uint64_t, MAX);
  CHECK_SADD(MAX, MIN, -1, int64_t, uint64_t, MAX);

  CHECK_SADD(MAX - 1, 1, MAX, int64_t, uint64_t, MAX);
  CHECK_SADD(MIN + 1, -1, MIN, int64_t, uint64_t, MAX);

  CHECK_SADD(MAX, 1, MAX, int64_t, uint64_t, MAX);
  CHECK_SADD(MAX, MAX, MAX, int64_t, uint64_t, MAX);
  CHECK_SADD(MIN, -1, MIN, int64_t, uint64_t, MAX);
  CHECK_SADD(MIN, MIN, MIN, int64_t, uint64_t, MAX);
}

int main(int argc, char** argv) {
  test_int8();
  test_int16();
  test_int32();
  test_int64();
  return 0;
}
