#include "chpl-atomics.h"
#include <assert.h>
#include <stdio.h>

#define test(type) { \
  int lockless; \
  atomic_ ## type a, b, c; \
  atomic_init_ ## type (&a, 1); \
  atomic_init_ ## type (&b, 2); \
  atomic_init_ ## type (&c, 3); \
  \
  lockless = atomic_is_lock_free_ ## type (&a); \
  \
  assert( 1 == atomic_load_ ## type (&a) ); \
  assert( 2 == atomic_load_ ## type (&b) ); \
  assert( 3 == atomic_load_ ## type (&c) ); \
  \
  atomic_store_ ## type (&a, 10); \
  atomic_store_ ## type (&b, 20); \
  atomic_store_ ## type (&c, 30); \
  assert( 10 == atomic_load_ ## type (&a) ); \
  assert( 20 == atomic_load_ ## type (&b) ); \
  assert( 30 == atomic_load_ ## type (&c) ); \
  \
  assert( 10 == atomic_exchange_ ## type (&a, 100) ); \
  assert( 100 == atomic_load_ ## type (&a) ); \
  \
  atomic_store_ ## type (&a, 10); \
  assert( ! atomic_compare_exchange_strong_ ## type (&a, 100, 11) ); \
  assert( atomic_compare_exchange_strong_ ## type (&a, 10, 11) ); \
  assert( 11 == atomic_load_ ## type (&a) ); \
  \
  atomic_store_ ## type (&a, 10); \
  assert( 10 == atomic_fetch_add_ ## type (&a, 2) ); \
  assert( 12 == atomic_load_ ## type (&a) ); \
  \
  atomic_store_ ## type (&a, 10); \
  assert( 10 == atomic_fetch_sub_ ## type (&a, 2) ); \
  assert( 8 == atomic_load_ ## type (&a) ); \
  \
  atomic_store_ ## type (&a, 0x12); \
  assert( 0x12 == atomic_fetch_or_ ## type (&a, 0x11) ); \
  assert( 0x13 == atomic_load_ ## type (&a) ); \
  \
  atomic_store_ ## type (&a, 0x12); \
  assert( 0x12 == atomic_fetch_xor_ ## type (&a, 0x10) ); \
  assert( 0x02 == atomic_load_ ## type (&a) ); \
  \
  atomic_store_ ## type (&a, 0x12); \
  assert( 0x12 == atomic_fetch_and_ ## type (&a, 0x10) ); \
  assert( 0x10 == atomic_load_ ## type (&a) ); \
}

int main(int argc, char** argv)
{
  atomic_bool flag;

  atomic_init_bool(&flag, true);
  atomic_store_bool(&flag, false);
  assert( false == atomic_exchange_bool(&flag, true) );
  assert( true == atomic_exchange_bool(&flag, true) );

  chpl_atomic_thread_fence(memory_order_seq_cst);
  chpl_atomic_signal_fence(memory_order_seq_cst);

  {
    atomic_uint_least8_t tmp;
    atomic_load_uint_least8_t(&tmp);
  }

  test(uint_least8_t);
  test(uint_least16_t);
  test(uint_least32_t);
  test(uint_least64_t);
  test(uintptr_t);

  printf("basic atomics test OK\n");

  return 0;
}
