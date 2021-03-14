#include <stdio.h>

#include "chpl-cache-support.c"

// These functions could go into chpl-cache-support.c
// but they aren't currently used by anything other
// than this test.

// Returns 1 if the passed entry is in the circlebuf, 0 otherwise.
static
int fifo_circleb_contained( int check_entry,
                            int first_entry,
                            int last_entry,
                            unsigned int entries_len)
{
  if( check_entry < 0 || check_entry >= (int) entries_len ) return 0;
  if( first_entry < 0 || last_entry < 0 ) {
    return 0;
  } else if( first_entry <= last_entry ) {
    // The easy order like
    // |  xxxxxxxxxx   |
    //   first    last
    return (first_entry <= check_entry && check_entry <= last_entry);
  } else {
    // A wrapped configuration like
    // | xxx      xxxxxxx |
    //    last   first
    return check_entry >= first_entry || check_entry <= last_entry;
  }
}

static int check_valid_for_skip_len(uint64_t* valid, uint64_t* test,
                                    uintptr_t skip, uintptr_t len,
                                    int mask_words)
{
  int j;
  memset(test, 0, sizeof(uint64_t)*mask_words);
  set_valids_for_skip_len(test,skip,len,mask_words);
  for( j = 0; j < mask_words; j++ ) {
    if( (valid[j] & test[j]) != test[j] ) {
      return 0;
    }
  }
  return 1;
}

// Testing routines.
static
void test_fifo_circleb(void)
{
  int first, last, evict;
  // Test fifo_circleb_push
  assert(fifo_circleb_count(-1, -1, 1024) == 0);
  assert(fifo_circleb_contained(0, -1, -1, 1024) == 0);
  first = -1; last = -1;
  evict = fifo_circleb_push(&first, &last, 1024);
  assert(evict == 0 && first == 0 && last == 0);

  evict = fifo_circleb_push(&first, &last, 1024);
  assert(evict == 0 && first == 0 && last == 1);

  evict = fifo_circleb_pop(&first, &last, 1024);
  assert(evict == 0 && first == 1 && last == 1);

  evict = fifo_circleb_pop(&first, &last, 1024);
  assert(evict == 1 && first == -1 && last == -1);

  first = -1; last = -1;
  evict = fifo_circleb_push(&first, &last, 1024);
  assert(evict == 0 && first == 0 && last == 0);


  assert(fifo_circleb_count(1, 1, 1024) == 1);
  assert(fifo_circleb_contained(0, 1, 1, 1024) == 0);
  assert(fifo_circleb_contained(1, 1, 1, 1024) == 1);
  assert(fifo_circleb_contained(2, 1, 1, 1024) == 0);
  first = 1; last = 1;
  evict = fifo_circleb_push(&first, &last, 1024);
  assert(evict == 0 && first == 1 && last == 2);

  assert(fifo_circleb_count(1, 2, 1024) == 2);
  assert(fifo_circleb_contained(0, 1, 2, 1024) == 0);
  assert(fifo_circleb_contained(1, 1, 2, 1024) == 1);
  assert(fifo_circleb_contained(2, 1, 2, 1024) == 1);
  assert(fifo_circleb_contained(3, 1, 2, 1024) == 0);
  first = 1; last = 2;
  evict = fifo_circleb_push(&first, &last, 1024);
  assert(evict == 0 && first == 1 && last == 3);

  assert(fifo_circleb_count(1023, 1023, 1024) == 1);
  assert(fifo_circleb_contained(1022, 1023, 1023, 1024) == 0);
  assert(fifo_circleb_contained(1023, 1023, 1023, 1024) == 1);
  assert(fifo_circleb_contained(0, 1023, 1023, 1024) == 0);
  assert(fifo_circleb_count(1023, 0, 1024) == 2);
  assert(fifo_circleb_contained(1022, 1023, 0, 1024) == 0);
  assert(fifo_circleb_contained(1023, 1023, 0, 1024) == 1);
  assert(fifo_circleb_contained(0, 1023, 0, 1024) == 1);
  assert(fifo_circleb_contained(1, 1023, 0, 1024) == 0);
  assert(fifo_circleb_count(1023, 1, 1024) == 3);
  assert(fifo_circleb_contained(1022, 1023, 1, 1024) == 0);
  assert(fifo_circleb_contained(1023, 1023, 1, 1024) == 1);
  assert(fifo_circleb_contained(0, 1023, 1, 1024) == 1);
  assert(fifo_circleb_contained(1, 1023, 1, 1024) == 1);
  assert(fifo_circleb_contained(2, 1023, 1, 1024) == 0);

  first = 1023; last = 1;
  evict = fifo_circleb_push(&first, &last, 1024);
  assert(evict == 0 && first == 1023 && last == 2);

  assert(fifo_circleb_count(1, 7, 8) == 7);

  first = 1; last = 7;
  evict = fifo_circleb_push(&first, &last, 8);
  assert(evict == 0 && first == 1 && last == 0);


  assert(fifo_circleb_count(0, 3, 4) == 4);

  first = 0; last = 3;
  evict = fifo_circleb_push(&first, &last, 4);
  assert(evict == 1 && first == 1 && last == 0);


  assert(fifo_circleb_count(1, 0, 4) == 4);

  first = 1; last = 0;
  evict = fifo_circleb_push(&first, &last, 4);
  assert(evict == 1 && first == 2 && last == 1);
}

static
void test_valid_bits(void)
{
#define TEST_NUM_WORDS 8
  uint64_t num;
  uint64_t bits[TEST_NUM_WORDS];
  uint64_t test[TEST_NUM_WORDS];
  uintptr_t skip, len;
  int got;

  num = 0;
  set_valid_bits64(&num, 0, 0);
  assert(num == 0);

  num = 0;
  set_valid_bits64(&num, 0, 64);
  assert(num == 0xffffffffffffffffUL);

  num = 0;
  set_valid_bits64(&num, 63, 1);
  assert(num == 0x8000000000000000UL);

  num = 0;
  set_valid_bits64(&num, 1, 1);
  assert(num == 0x0000000000000002UL);

  set_valid_bits64(&num, 3, 1);
  assert(num == 0x000000000000000aUL);

  num = (uint64_t) -1;
  assert(0 == count_valid_before(&num, 0, 1));
  assert(1 == count_valid_before(&num, 1, 1));
  assert(63 == count_valid_before(&num, 63, 1));
  assert(64 == count_valid_before(&num, 64, 1));

  assert(64 == count_valid_at_after(&num, 0, 1));
  assert(63 == count_valid_at_after(&num, 1, 1));
  assert(62 == count_valid_at_after(&num, 2, 1));
  assert(1 == count_valid_at_after(&num, 63, 1));
  assert(0 == count_valid_at_after(&num, 64, 1));

  memset(bits, 0, sizeof(bits));
  set_valids_for_skip_len(bits, 1, 1, TEST_NUM_WORDS); // 1 is set
  assert(check_valid_for_skip_len(bits, test, 1, 1, TEST_NUM_WORDS));
  assert(!check_valid_for_skip_len(bits, test, 0, 1, TEST_NUM_WORDS));
  got = get_skip_len_for_valids(bits, 0, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 1 && len == 1);
  got = get_skip_len_for_valids(bits, 1, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 1 && len == 1);
  assert(any_set_for_skip_len(bits, 0, 10, TEST_NUM_WORDS));
  assert(!any_set_for_skip_len(bits, 0, 1, TEST_NUM_WORDS));
  assert(any_set_for_skip_len(bits, 1, 1, TEST_NUM_WORDS));
  assert(any_set_for_skip_len(bits, 1, 2, TEST_NUM_WORDS));
  assert(any_set_for_skip_len(bits, 0, 2, TEST_NUM_WORDS));
  assert(!any_set_for_skip_len(bits, 2, 10, TEST_NUM_WORDS));
  assert(all_set_for_skip_len(bits, 1, 1, TEST_NUM_WORDS));

  assert(0 == count_valid_before(bits, 0, TEST_NUM_WORDS));
  assert(0 == count_valid_before(bits, 1, TEST_NUM_WORDS));
  assert(1 == count_valid_before(bits, 2, TEST_NUM_WORDS));
  assert(1 == count_valid_before(bits, 64, TEST_NUM_WORDS));
  assert(1 == count_valid_before(bits, 128, TEST_NUM_WORDS));
  assert(1 == count_valid_at_after(bits, 1, TEST_NUM_WORDS));
  assert(0 == count_valid_at_after(bits, 2, TEST_NUM_WORDS));
  assert(0 == count_valid_at_after(bits, 3, TEST_NUM_WORDS));
  assert(0 == count_valid_at_after(bits, 64, TEST_NUM_WORDS));
  assert(0 == count_valid_at_after(bits, 128, TEST_NUM_WORDS));


  set_valids_for_skip_len(bits, 1, 2, TEST_NUM_WORDS); // 12 is set
  assert(check_valid_for_skip_len(bits, test, 1, 1, TEST_NUM_WORDS));
  assert(check_valid_for_skip_len(bits, test, 2, 1, TEST_NUM_WORDS));
  assert(check_valid_for_skip_len(bits, test, 1, 2, TEST_NUM_WORDS));
  got = get_skip_len_for_valids(bits, 0, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 1 && len == 2);
  got = get_skip_len_for_valids(bits, 1, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 1 && len == 2);
  got = get_skip_len_for_valids(bits, 2, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 2 && len == 1);
  assert(any_set_for_skip_len(bits, 0, 10, TEST_NUM_WORDS));
  assert(!any_set_for_skip_len(bits, 0, 1, TEST_NUM_WORDS));
  assert(any_set_for_skip_len(bits, 1, 2, TEST_NUM_WORDS));
  assert(!any_set_for_skip_len(bits, 3, 2, TEST_NUM_WORDS));
  assert(all_set_for_skip_len(bits, 1, 2, TEST_NUM_WORDS));
  assert(!all_set_for_skip_len(bits, 0, 2, TEST_NUM_WORDS));

  set_valids_for_skip_len(bits, 0, 2, TEST_NUM_WORDS); // 012 is set
  assert(check_valid_for_skip_len(bits, test, 0, 2, TEST_NUM_WORDS));
  assert(check_valid_for_skip_len(bits, test, 0, 3, TEST_NUM_WORDS));
  got = get_skip_len_for_valids(bits, 0, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 0 && len == 3);
  got = get_skip_len_for_valids(bits, 1, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 1 && len == 2);
  got = get_skip_len_for_valids(bits, 2, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 2 && len == 1);
  assert(any_set_for_skip_len(bits, 0, 3, TEST_NUM_WORDS));
  assert(any_set_for_skip_len(bits, 0, 4, TEST_NUM_WORDS));
  assert(all_set_for_skip_len(bits, 0, 3, TEST_NUM_WORDS));
  assert(!all_set_for_skip_len(bits, 0, 4, TEST_NUM_WORDS));

  set_valids_for_skip_len(bits, 4, 1, TEST_NUM_WORDS); // 012x4 is set
  assert(check_valid_for_skip_len(bits, test, 4, 1, TEST_NUM_WORDS));
  assert(!check_valid_for_skip_len(bits, test, 3, 1, TEST_NUM_WORDS));
  assert(!check_valid_for_skip_len(bits, test, 3, 2, TEST_NUM_WORDS));
  got = get_skip_len_for_valids(bits, 0, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 0 && len == 3);
  got = get_skip_len_for_valids(bits, 3, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 4 && len == 1);
  assert(!any_set_for_skip_len(bits, 3, 1, TEST_NUM_WORDS));
  assert(any_set_for_skip_len(bits, 0, 5, TEST_NUM_WORDS));
  assert(!all_set_for_skip_len(bits, 0, 5, TEST_NUM_WORDS));

  assert(0 == count_valid_before(bits, 0, TEST_NUM_WORDS));
  assert(1 == count_valid_before(bits, 1, TEST_NUM_WORDS));
  assert(2 == count_valid_before(bits, 2, TEST_NUM_WORDS));
  assert(3 == count_valid_before(bits, 3, TEST_NUM_WORDS));
  assert(3 == count_valid_before(bits, 4, TEST_NUM_WORDS));
  assert(4 == count_valid_before(bits, 5, TEST_NUM_WORDS));
  assert(4 == count_valid_before(bits, 6, TEST_NUM_WORDS));
  assert(4 == count_valid_at_after(bits, 0, TEST_NUM_WORDS));
  assert(3 == count_valid_at_after(bits, 1, TEST_NUM_WORDS));
  assert(2 == count_valid_at_after(bits, 2, TEST_NUM_WORDS));
  assert(1 == count_valid_at_after(bits, 3, TEST_NUM_WORDS));
  assert(1 == count_valid_at_after(bits, 4, TEST_NUM_WORDS));
  assert(0 == count_valid_at_after(bits, 5, TEST_NUM_WORDS));
  assert(0 == count_valid_at_after(bits, 6, TEST_NUM_WORDS));


  set_valids_for_skip_len(bits, 64, 64, TEST_NUM_WORDS); // 012x4 is set, then 64..127 are set
  assert(check_valid_for_skip_len(bits, test, 64, 64, TEST_NUM_WORDS));
  got = get_skip_len_for_valids(bits, 0, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 0 && len == 3);
  got = get_skip_len_for_valids(bits, 3, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 4 && len == 1);
  got = get_skip_len_for_valids(bits, 5, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 64 && len == 64);
  assert(any_set_for_skip_len(bits, 64, 64, TEST_NUM_WORDS));
  assert(any_set_for_skip_len(bits, 16, 64, TEST_NUM_WORDS));
  assert(!any_set_for_skip_len(bits, 128, 64, TEST_NUM_WORDS));
  assert(all_set_for_skip_len(bits, 64, 64, TEST_NUM_WORDS));
  assert(!all_set_for_skip_len(bits, 16, 64, TEST_NUM_WORDS));

  set_valids_for_skip_len(bits, 129, 256, TEST_NUM_WORDS); // 012x4 is set, then 64..127 and 129..384 are set
  assert(check_valid_for_skip_len(bits, test, 129, 256, TEST_NUM_WORDS));
  assert(!check_valid_for_skip_len(bits, test, 128, 1, TEST_NUM_WORDS));
  assert(check_valid_for_skip_len(bits, test, 64, 64, TEST_NUM_WORDS));
  got = get_skip_len_for_valids(bits, 0, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 0 && len == 3);
  got = get_skip_len_for_valids(bits, 3, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 4 && len == 1);
  got = get_skip_len_for_valids(bits, 5, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 64 && len == 64);
  got = get_skip_len_for_valids(bits, 128, &skip, &len, TEST_NUM_WORDS);
  assert(got && skip == 129 && len == 256);
  assert(any_set_for_skip_len(bits, 129, 256, TEST_NUM_WORDS));
  assert(all_set_for_skip_len(bits, 129, 256, TEST_NUM_WORDS));

  assert(0 == count_valid_before(bits, 0, TEST_NUM_WORDS));
  assert(1 == count_valid_before(bits, 1, TEST_NUM_WORDS));
  assert(3 == count_valid_before(bits, 4, TEST_NUM_WORDS));
  assert(4 == count_valid_before(bits, 64, TEST_NUM_WORDS));
  assert(5 == count_valid_before(bits, 65, TEST_NUM_WORDS));
  assert(68 == count_valid_before(bits, 128, TEST_NUM_WORDS));
  assert(195 == count_valid_before(bits, 256, TEST_NUM_WORDS));
  assert(324 == count_valid_before(bits, 64*TEST_NUM_WORDS, TEST_NUM_WORDS));
  assert(324 == count_valid_at_after(bits, 0, TEST_NUM_WORDS));
  assert(323 == count_valid_at_after(bits, 1, TEST_NUM_WORDS));
  assert(322 == count_valid_at_after(bits, 2, TEST_NUM_WORDS));
  assert(321 == count_valid_at_after(bits, 3, TEST_NUM_WORDS));
  assert(321 == count_valid_at_after(bits, 4, TEST_NUM_WORDS));
  assert(320 == count_valid_at_after(bits, 5, TEST_NUM_WORDS));
  assert(320 == count_valid_at_after(bits, 6, TEST_NUM_WORDS));
  assert(320 == count_valid_at_after(bits, 64, TEST_NUM_WORDS));
  assert(319 == count_valid_at_after(bits, 65, TEST_NUM_WORDS));
  assert(318 == count_valid_at_after(bits, 66, TEST_NUM_WORDS));
 
#undef TEST_NUM_WORDS
}


static void chpl_cache_support_test(void)
{
  assert(round_down_to_mask(0, 15) == 0);
  assert(round_down_to_mask(10, 15) == 0);
  assert(round_down_to_mask(15, 15) == 0);
  assert(round_down_to_mask(16, 15) == 16);
  assert(round_down_to_mask(17, 15) == 16);
  assert(round_up_to_mask(0, 15) == 0);
  assert(round_up_to_mask(1, 15) == 16);
  assert(round_up_to_mask(10, 15) == 16);
  assert(round_up_to_mask(15, 15) == 16);
  assert(round_up_to_mask(16, 15) == 16);
  assert(round_up_to_mask(17, 15) == 32);

  test_fifo_circleb();
  test_valid_bits();
}

int main(int argc, char** argv)
{
  chpl_cache_support_test();
  printf("All tests passed\n");
  return 0;
}

