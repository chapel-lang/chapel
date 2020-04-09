/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include "chpl-bitops.h"
#include "chpl-align.h"

// ----------  SUPPORT FUNCTIONS 
typedef int64_t cache_seqn_t;
#define NO_SEQUENCE_NUMBER 0
static inline
cache_seqn_t seqn_min(cache_seqn_t a, cache_seqn_t b)
{
  if( a != NO_SEQUENCE_NUMBER && a < b ) return a;
  return b;
}
static inline
cache_seqn_t seqn_max(cache_seqn_t a, cache_seqn_t b)
{
  if( a != NO_SEQUENCE_NUMBER && a > b ) return a;
  return b;
}

/*
typedef uint16_t saturating_count_t;
#define SATURATING_COUNT_T_MAX 0xffff

static inline void saturating_increment(saturating_count_t *x) {
  saturating_count_t v = *x;
  if( v < SATURATING_COUNT_T_MAX ) v++;
  *x = v;
}
static inline void saturating_add(saturating_count_t *x, unsigned int num) {
  saturating_count_t v = *x;
  unsigned long bigv = v;
  bigv += num;
  if( bigv <= SATURATING_COUNT_T_MAX ) v = bigv;
  else v = SATURATING_COUNT_T_MAX;
  *x = v;
}
*/

// Single linked list/stack
#define SINGLE_POP_HEAD(tree, NAME) { \
  if( tree->NAME##_head ) { \
    tree->NAME##_head = tree->NAME##_head->next; \
  } \
}
#define SINGLE_PUSH_HEAD(tree, element, NAME) { \
  element->next = tree->NAME##_head; \
  tree->NAME##_head = element; \
}

// doubly linked list/queue
#define DOUBLE_REMOVE_TAIL(tree, NAME) { \
  if( tree->NAME##_tail ) { \
    /* if we aren't the only item in the list, the previous entry exists */ \
    /* and should have its next pointer set to NULL */  \
    if (tree->NAME##_tail->prev) { \
      tree->NAME##_tail->prev->next = NULL; \
    } \
    /* if we were the only item in the list, make the head point to NULL */ \
    if (tree->NAME##_head == tree->NAME##_tail) { \
      tree->NAME##_head = NULL; \
    } \
    /* make the tail point at the previous entry (which may be NULL, */ \
    /* and that's fine) */ \
    tree->NAME##_tail = tree->NAME##_tail->prev; \
  } \
}

#define DOUBLE_REMOVE(tree, element, NAME) { \
  /* if the tail element is being removed, update the tail pointer  */ \
  if (tree->NAME##_tail == element) { \
    tree->NAME##_tail = element->prev; \
  } \
  /* if the head element is being removed, update the head pointer */ \
  if (tree->NAME##_head == element) { \
    tree->NAME##_head = element->next; \
  } \
  /* if there is a previous element, take this element out of the forward list. */ \
  if (element->prev) { \
    element->prev->next = element->next; \
  } \
  /* if there is a next element, take this element out of the backward list. */ \
  if (element->next) { \
    element->next->prev = element->prev; \
  } \
}

#define DOUBLE_PUSH_HEAD(tree, element, NAME) { \
  /* set the soon-to-be-new-head's next pointer to the current head */ \
  element->next = tree->NAME##_head; \
  /* new head will have no previous element */ \
  element->prev = NULL; \
  /* if the list isn't empty, update the head's prev pointer */ \
  if (tree->NAME##_head) { \
    tree->NAME##_head->prev = element; \
  } \
  /* point the head at the new element */ \
  tree->NAME##_head = element; \
  if (NULL == tree->NAME##_tail) { \
    tree->NAME##_tail = element; \
  } \
}
#define DOUBLE_PUSH_TAIL(tree, element, NAME) { \
  /* set the soon-to-be-new-tail's prev pointer to the current tail */ \
  element->prev = tree->NAME##_tail; \
  /* new tail will have no next element */ \
  element->next = NULL; \
  /* if the list isn't empty, update the tail's next pointer */ \
  if (tree->NAME##_tail) { \
    tree->NAME##_tail->next = element; \
  } \
  /* point the tail at the new element */ \
  tree->NAME##_tail = element; \
  if (NULL == tree->NAME##_head) { \
    tree->NAME##_head = element; \
  } \
}

// FIFO Circular Bounded Buffer routines

//  <--                  <--                   <--                       <-- 
//  tail              complete                 start                     head
//  "first entry"                                                "last entry"
//   evict from this end                                  enqueue on this end
 
// Adds an element to a FIFO circle buffer.
// When completed:
//  - first_entry and last_entry will be updated
//  - the new element goes in last_entry
//  - if the return value is 1, there is an existing element in last_entry
//    that should be evicted.
static inline
int fifo_circleb_push(int* first_entry_p, int* last_entry_p, unsigned int entries_len)
{
  int first_entry = *first_entry_p;
  int last_entry = *last_entry_p;
  unsigned int entries_mask = entries_len - 1;
  int num_entries_to_consume = 0;

  // Assert that entries_len is a power of 2.
  assert( entries_len > 0 );
  assert( ! (entries_len & entries_mask));

  if( first_entry < 0 || last_entry < 0 ) {
    // This will be the first entry.
    *first_entry_p = 0;
    *last_entry_p = 0;
    return 0; // no need to evict.
  }

  // Make sure that we consume an entry if last_entry+1 == first_entry.
  if( ((last_entry+1) & entries_mask) == first_entry &&
      num_entries_to_consume == 0 ) {
    num_entries_to_consume++;
  }

  // Move first-entry forward num_to_consume elements.
  *first_entry_p = (first_entry+num_entries_to_consume) & entries_mask;
  // Last entry will be the entry we are currently adding.
  *last_entry_p = (last_entry+1) & entries_mask;
  return num_entries_to_consume;
}

// Removes an element from a FIFO circle buffer.
// The index of the removed element is returned, or -1 if there were no elements.
static inline
int fifo_circleb_pop( int* first_entry_p, int* last_entry_p, unsigned int entries_len)
{
  int first_entry = *first_entry_p;
  int last_entry = *last_entry_p;
  unsigned int entries_mask = entries_len - 1;

  // Assert that entries_len is a power of 2.
  assert( entries_len > 0 );
  assert( ! (entries_len & entries_mask));

  if( first_entry < 0 || last_entry < 0 ) {
    return -1; // nothing to remove.
  }

  if( first_entry == last_entry ) {
    // only one entry
    *first_entry_p = -1;
    *last_entry_p = -1;
    return first_entry;
  }

  *first_entry_p = (first_entry+1) & entries_mask;
  return first_entry;
}


// Returns the number of entries in a circlebuf.
static
int fifo_circleb_count( int first_entry,
                        int last_entry,
                        unsigned int entries_len)
{
  if( first_entry < 0 || last_entry < 0 ) {
    return 0;
  } else if( first_entry <= last_entry ) {
    // The easy order like
    // |  xxxxxxxxxx   |
    //   first    last
    return last_entry - first_entry + 1;
  } else {
    // A wrapped configuration like
    // | xxx      xxxxxxx |
    //    last   first
    return entries_len - first_entry + last_entry + 1;
  }
}

static
int fifo_circleb_isfull( int first_entry,
                         int last_entry,
                         unsigned int entries_len)
{
  return fifo_circleb_count(first_entry,last_entry,entries_len) == entries_len;
}

static inline
void set_valid_bits64(uint64_t* valid, uintptr_t skip, uintptr_t len)
{
  uint64_t one = 1;
  if( len == 64 ) {
    *valid |= (uint64_t)-1;
  } else {
    *valid |= (((one << len) - 1) << skip);
  }
}
static inline
void set_valids_for_skip_len(uint64_t* valid, uintptr_t skip, uintptr_t len, int mask_words)
{
  int i;
  uintptr_t offset;
  uintptr_t use_end;
  intptr_t use_start;

  offset = 0;
  for( i = 0; i < mask_words; i++ ) {
    // valid[i] stores the valid bits for offset..offset+63
    if( offset + 64 <= skip ) {
      // entirely before the region -- nothing valid
    } else if( offset > skip + len ) {
      // entirely after the region -- nothing valid
    } else if( skip <= offset && offset + 64 <= skip+len ) {
      // entirely within the region -- totally valid
      valid[i] = (uint64_t) -1;
    } else {
      use_start = skip - offset;
      if( use_start < 0 ) use_start = 0;
      use_end = 64;
      if( offset + 64 >= skip + len ) use_end = skip + len - offset;
      set_valid_bits64(&valid[i], use_start, use_end - use_start);
    }
    offset += 64;
  }
}

static
void unset_valids_for_skip_len(uint64_t* valid, uint64_t* tmp, 
                              uintptr_t skip, uintptr_t len, int mask_words)
{
  int j;
  memset(tmp, 0, sizeof(uint64_t)*mask_words);
  set_valids_for_skip_len(tmp,skip,len,mask_words);
  for( j = 0; j < mask_words; j++ ) {
    // if the bit is set in tmp, it must
    // not be set anymore in valid...
    valid[j] &= ~tmp[j];
  }
}

// Assuming start is the next position after the last return (skip+len), or 0,
// this function returns the next region of valid bits in *skip and *len and returns 1,
// or returns 0 if no region was found.
static
int get_skip_len_for_valids(uint64_t* valid, uintptr_t start,
                            uintptr_t* skip, uintptr_t* len, int mask_words)
{
  int i;
  int first_one;
  int first_zero;
  uint64_t x;
  uint64_t all_ones = (uint64_t) -1;

  if( start >= mask_words*64 ) return 0;

  // Find the end of the run of 0s starting at bit position start.
  i = start/64;
  x = valid[i] >> (start % 64);
  if( x != 0 ) {
    first_one = 64*i + (start % 64) + chpl_bitops_ctz_64(x);
    chpl_bitops_ctz_64(x);
  } else {
    for( i++; i < mask_words && valid[i] == 0; i++ ) ;
    if( i == mask_words ) return 0;
    // otherwise, we have valid[i] != 0
    first_one = 64*i + chpl_bitops_ctz_64(valid[i]);
  }

  // Find the end of the run of 1s starting at bit position first_one
  i = first_one/64;
  x = valid[i] >> (first_one % 64);
  if( x != (all_ones >> (first_one % 64)  )) {
    first_zero = 64*i + (first_one % 64) + chpl_bitops_ctz_64(~x);
  } else {
    for( i++; i < mask_words && valid[i] == (uint64_t)-1; i++ ) ;
    if( i == mask_words ) {
      first_zero = 64 * mask_words;
    } else {
      // otherwise, we have valid[i] != -1
      first_zero = 64*i + chpl_bitops_ctz_64(~valid[i]);
    }
  }

  *skip = first_one;
  *len = first_zero - first_one;
  return 1;
}

static
int any_set_for_skip_len(uint64_t* valid, uintptr_t skip, uintptr_t len, int mask_words)
{
  uintptr_t gotskip, gotlen;
  int got;
  // Get the start and length of the next region of 1s starting at skip
  got = get_skip_len_for_valids(valid, skip, &gotskip, &gotlen, mask_words);
  // If we didn't find any such region, not set
  if( !got ) return 0;
  // If the region of 1s started after skip+len, not set
  if( gotskip >= skip + len ) return 0;
  // Otherwise, we must have found something set.
  return 1;
}

static
int all_set_for_skip_len(uint64_t* valid, uintptr_t skip, uintptr_t len, int mask_words)
{
  uintptr_t gotskip, gotlen;
  int got;
  // Get the start and length of the next region of 1s starting at skip
  got = get_skip_len_for_valids(valid, skip, &gotskip, &gotlen, mask_words);
  return got && skip == gotskip && gotlen >= len;
}

// Returns the number of set bits in valid before bit position at.
static
int count_valid_before(uint64_t* valid, uintptr_t at, int mask_words)
{
  int i;
  int ati;
  int count;

  ati = at/64;
  count = 0;

  // Count bits in words totally before at.
  for( i = 0; i < mask_words && i < ati; i++ ) {
    count += chpl_bitops_popcount_64(valid[i]);
  }

  // Count bits in valid[ati]
  if( at%64 != 0 )
    count += chpl_bitops_popcount_64(valid[ati] << (64 - (at%64)));
  return count;
}

// Returns the number of set bits in valid at or after bit position at.
static
int count_valid_at_after(uint64_t* valid, uintptr_t at, int mask_words)
{
  int i;
  int ati;
  int count;

  ati = at/64;
  count = 0;
  // Count bits in valid[ati]
  if( ati < mask_words ) {
    count += chpl_bitops_popcount_64(valid[ati] >> (at%64));
  }
  // Count bits in words totally after at.
  for( i = ati + 1; i < mask_words; i++ ) {
    count += chpl_bitops_popcount_64(valid[i]);
  }
  return count;
}
 
