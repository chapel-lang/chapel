#include <stdlib.h>
#include <assert.h>

#include "chpl-bitops.h"

int main(int argc, char** argv) {
  // 32 bit
  uint32_t ones32 = 0xFFFFFFFF;
  uint32_t zero32 = 0;
  uint32_t mask32 = 0x1;

  assert(chpl_bitops_popcount_32(zero32) == 0);
  assert(chpl_bitops_clz_32(zero32) == 32);
  assert(chpl_bitops_ctz_32(zero32) == 0);
  assert(chpl_bitops_parity_32(zero32) == 0);

  assert(chpl_bitops_popcount_32(ones32) == 32);
  assert(chpl_bitops_clz_32(ones32) == 0);
  assert(chpl_bitops_ctz_32(ones32) == 0);
  assert(chpl_bitops_parity_32(ones32) == 0);

  // check 1, 2, 4, ...
  for (uint32_t i=0; i<32; i++) {
    assert(chpl_bitops_popcount_32(mask32 << i) == 1);
    assert(chpl_bitops_clz_32(mask32 << i) == 31-i);
    assert(chpl_bitops_ctz_32(mask32 << i) == i);
    assert(chpl_bitops_parity_32(mask32 << i) == 1);
  }

  // check ~1
  assert(chpl_bitops_popcount_32(~mask32) == 31);
  assert(chpl_bitops_clz_32(~mask32) == 0);
  assert(chpl_bitops_ctz_32(~mask32) == 1);
  assert(chpl_bitops_parity_32(~mask32) == 1);

  // check ~2, ~4, ..., ~(1<<30)
  for (uint32_t i=1; i<31; i++) {
    assert(chpl_bitops_popcount_32(~(mask32 << i)) == 31);
    assert(chpl_bitops_clz_32(~(mask32 << i)) == 0);
    assert(chpl_bitops_ctz_32(~(mask32 << i)) == 0);
    assert(chpl_bitops_parity_32(~(mask32 << i)) == 1);
  }

  // check ~(1<<31)
  assert(chpl_bitops_popcount_32(~(mask32 << 31)) == 31);
  assert(chpl_bitops_clz_32(~(mask32 << 31)) == 1);
  assert(chpl_bitops_ctz_32(~(mask32 << 31)) == 0);
  assert(chpl_bitops_parity_32(~(mask32 << 31)) == 1);

  // 64 bit
  uint64_t ones64 = 0xFFFFFFFFFFFFFFFF;
  uint64_t zero64 = 0;
  uint64_t mask64 = 0x1;

  assert(chpl_bitops_popcount_64(zero64) == 0);
  assert(chpl_bitops_clz_64(zero64) == 64);
  assert(chpl_bitops_ctz_64(zero64) == 0);
  assert(chpl_bitops_parity_64(zero64) == 0);

  assert(chpl_bitops_popcount_64(ones64) == 64);
  assert(chpl_bitops_clz_64(ones64) == 0);
  assert(chpl_bitops_ctz_64(ones64) == 0);
  assert(chpl_bitops_parity_64(ones64) == 0);

  // check 1, 2, 4, ...
  for (uint64_t i=0; i<64; i++) {
    assert(chpl_bitops_popcount_64(mask64 << i) == 1);
    assert(chpl_bitops_clz_64(mask64 << i) == 63-i);
    assert(chpl_bitops_ctz_64(mask64 << i) == i);
    assert(chpl_bitops_parity_64(mask64 << i) == 1);
  }

  // check ~1
  assert(chpl_bitops_popcount_64(~mask64) == 63);
  assert(chpl_bitops_clz_64(~mask64) == 0);
  assert(chpl_bitops_ctz_64(~mask64) == 1);
  assert(chpl_bitops_parity_64(~mask64) == 1);

  // check ~2, ~4, ..., ~(1<<62)
  for (uint64_t i=1; i<63; i++) {
    assert(chpl_bitops_popcount_64(~(mask64 << i)) == 63);
    assert(chpl_bitops_clz_64(~(mask64 << i)) == 0);
    assert(chpl_bitops_ctz_64(~(mask64 << i)) == 0);
    assert(chpl_bitops_parity_64(~(mask64 << i)) == 1);
  }

  // check ~(1<<63)
  assert(chpl_bitops_popcount_64(~(mask64 << 63)) == 63);
  assert(chpl_bitops_clz_64(~(mask64 << 63)) == 1);
  assert(chpl_bitops_ctz_64(~(mask64 << 63)) == 0);
  assert(chpl_bitops_parity_64(~(mask64 << 63)) == 1);


  return 0;
}
