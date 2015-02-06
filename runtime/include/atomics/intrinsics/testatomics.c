/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "chpl-atomics.h"
#include <assert.h>

#define test(type) { \
  bool lockless; \
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
  atomic_flag flag;

  atomic_flag_clear(&flag);
  assert( false == atomic_flag_test_and_set(&flag) );
  assert( true == atomic_flag_test_and_set(&flag) );

  atomic_thread_fence(memory_order_seq_cst);
  atomic_signal_thread_fence(memory_order_seq_cst);

  {
    atomic_uint_least8_t tmp;
    atomic_load_uint_least8_t(&tmp);
  }
  /*
  test(uint_least8_t);
  test(uint_least16_t);
  test(uint_least32_t);
  test(uint_least64_t);
  test(uintptr_t);
  */
  return 0;
}
