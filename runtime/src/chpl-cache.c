/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

/* Remote Data Cache for Chapel.

== Introduction ==

It's easy to write Chapel programs that create many small messages. For example:

   var A:[1..100] int;
   ....
   on Locales[1] {
     for a in A {
       use a
     }
   }

That bit of code might be for example using the vector in a distributed
matrix-vector multiply. Or maybe it's just some poorly written code. Either
way, it results in a 100 8-byte GETs from Locale 1 to Locale 0.

One way around this problem would be to create a local copy of A, enabling the
Chapel compiler's ability to optimize whole array assignment. While this
approach does reduce the number of messages, and is probably the right idea in
a library context, it has a few drawbacks:

  - the obvious solution might use too much memory (imagine than A is large and
    distributed; we can't just copy the whole thing).
  - a tiled/blocked solution adds complexity.
  - if many such local copies are used throughout a program, we might end
    up with too much memory used for buffering, especially on large systems.
  - as a communication optimization that would need to be located within the
    code using A, it diminishes the Chapel promise of separating algorithm
    from data distribution.

== Caching Remote Data ==

The basic idea here is to add a new runtime component that caches remote data.
This cache allows for:

  - data re-use without adding communication
  - automatic prefetching triggered by sequential access
  - automatic write-behind for puts
  - user- (or compiler-) requested prefetching similar to __builtin_prefetch
  - bounded communication buffers when the cache is used

Philosophically, the cache needs to exist on each locale, but in the
implementation (as we'll discuss further below) there is one cache per pthread.

What about memory consistency? The natural and key question one should ask
about any cache is how it ensures that the program still runs in a sane manner.
More specifically, you might want to know the answers to these three questions:

  1) Do non-parallel programs still work as they would without the cache?
  2) How can tasks on two locales ever share data?
  3) Does the cache introduce a lot of memory consistency traffic?

The key idea in the design of this cache is to avoid all memory consistency
traffic. Instead, all data in the cache is considered invalid once the memory
consistency model requires us to read new values. But more on that in a second;
let's discuss the answer to first two questions.

For non-parallel programs (or program regions), reads and writes happen through
the cache. Naturally, a written value that is then read needs to return the
value written (and not some other value). Then, at some point, the written
value needs to end up in a PUT to the destination locale. The main wrinkle here
for Chapel is that 'on' statements have specific meaning within the cache. When
we begin an 'on' statement, we must invalidate everything in the cache (since a
sequential program could have written values there in another locale before the
'on' statement began). When we complete an 'on' statement, we must flush any
pending PUTs in the cache, since the locale we return to might rely on the
values being written there.

How do tasks share data? The short answer is - they synchronize. Suppose one
task wants to write a value to memory, then ask the second task to read the
newly written value. As with processor-cache based memory consistency, the two
tasks need to do the following in order to orchestrate this:

x = 0 at program start.

task A, producer
   x = 43
   // write behind x?
   release barrier
   notify task B

task B, consumer
  // prefetch x?
  wait for notification
  acquire barrier
  read x and see x == 43

** Note -- We will use the words 'barrier,' 'memory barrier,' and 'fence'
  interchangeably. Since we're talking about a cache and memory consistency, it
  should be clear that we are NOT talking about the distributed computing barrier
  in which tasks wait for all tasks to arrive at the same point.

What do 'acquire' and 'release' mean? These terms are from the C++11 and C11
memory model. 'acquire' means that any reads used after the barrier cannot be
started before the barrier.  The problem that the acquire barrier is addressing
is that we might have read ahead some data some time ago (either with
prefetching or just cache reuse) that is no longer usable. In the example, we
have highlighted this situation with the 'prefetch x?' comment in task B. In
the fictional, ideal world, all of our GETs are prefetched into the cache to
cover network latency.  But suppose that the prefetch happened before task A
completed its write to x. Then task B cannot use the value for x in the cache -
if it did, it would have a stale old value. The acquire barrier indicates that
the old value for x (or any other memory location) is not acceptable. For this
cache in particular, an acquire barrier means that any value in the cache is no
longer usable - in other words, an acquire barrier is equivalent to evicting
everything from the cache.

The release barrier is handling the opposite situation. 'release' means that
any writes started before the barrier must complete before the barrier. In the
example, the comment after x = 43 in task A says 'write behind x?'. It would be
nice if we could take our time writing x, so that we can combine it with other
writes or just cover network latency. The problem is - if we are able to put
off completing the writing of x until after task B is notified, it could read
the old value. So, the 'release' barrier indicates that any writes performed by
the program need to be totally completed.

Note that 'acquire' and 'release' memory barriers are necessary for
multithreaded programming *on a single locale*. So the memory semantics that
programs rely on *already* are sufficient to enable the cache of remote data.

Note also that many operations in Chapel come with acquire and/or release
barriers. At the moment, sync variables and atomic operations have full barrier
semantics (meaning that they imply an acquire and a release barrier). So in a
real Chapel program, the required barriers would probably be hidden in whatever
technique was used to 'notify' or 'wait'. Atomic operations on atomic-type
variables can specify the required memory semantics (e.g., you could do
myatomic.fetchAdd(1, memory_order_acquire) but without the order= argument
you get a full barrier ).

== Implementation Notes ==

The cache itself is a 2Q cache (because this kind of cache is reported to have
better efficiency than a plain LRU). Besides being in a queue of one sort or
another, entries are also stored in a 'pointer tree' which is a two-level
'hashtable' where the hash function just selects different portions of the
remote address. The pointer tree uses separate chaining (ie, each hash table
element is actually a linked list of elements that go into that bucket).

The cache consists of 'cache entries', one per 'cache page'. A 'cache page' is
1024 bytes in the current implementation. The pointer tree and the 2Q queues
consist of cache entries which may point to a 1024-byte cache page. However, a
GET is always rounded up to entire 'cache line'. A cache line is currently 64
bytes. Each cache entry tracks which cache lines are valid (ie, for which cache
lines in the cache page have we done a GET?) and for pages that have been
written to in a PUT - aka 'dirty pages' - which bytes in the page have been
written to.

There is a tradeoff in the cache line size and in the cache page size:
  - smaller cache lines might mean lower latency gets
  - larger cache lines might mean fewer gets/more cache hits
  - smaller cache pages mean less wasted space (imagine putting 1 byte per page)
  - larger cache pages mean larger aggregated operations
    (since read ahead/write behind will only ever get a single cache page
     in one operation).

We chose 64 bytes for the cache line size because on our Infiniband network, 64
bytes is the largest request size which has no significant increase in latency
from an 8 byte request. We chose 1024 bytes for the cache page size because it
is the smallest request size that allows close to peak bandwidth in our
network.

When processing a GET, we first check to see if the requested cache page is
in the pointer tree. If not, we find an unused cache page and immediately start
a nonblocking get into the appropriate portion of that page. While the get is
ongoing, we adjust the pointer tree and the 2Q queues in order to add a new
cache entry. If we did find the requested cache page in the pointer tree, we
check to see if the requested data is in the page's dirty bits or valid lines.
If so, we copy out the cached data. If not, we wait for any pending operations
that would overlap with the get we are about to start and then we start a
nonblocking get and mark the appropriate lines as valid. The implementation
finishes up a GET operation by waiting for the nonblocking get we started
(unless we are doing a prefetch, in which case it is not necessary to wait),
then by copying the newly gotten data out of the cache and into the request
buffer.

When processing GETs on adjacent memory locations, the cache triggers
both synchronous and asynchronous read-ahead.

When processing a PUT, we similarly check for the requested cache page in the
pointer tree and use an unused page if not. We find a unused 'dirty entry' to
track the dirty bits of the cache page if the cache entry does not already have
one. If we are working with an existing cache entry, we wait for any operation
that would overlap with the region of the cache page we are about to overwrite.
We copy the data into the appropriate part of the cache page and mark the
appropriate bits in the dirty entry.

On a release barrier or when we have too many 'dirty entries', we go through
dirty pages and create and start PUTs for each contiguous section with the dirty
bits set. In this manner, PUTs to adjacent memory locations are aggregated.

Note that it took significant effort to implement this cache efficiently
enough.  The implementation we are presenting here is the 5th design we tried.

Earlier designs had these problems:
 -- used one cache per task (too much memory)
 -- used one cache per locale (too high cache access cost b/c of sync)
 -- used both task-local and pthread-local caches (too complex)
 -- allocated memory per request (too slow / too much latency overhead)
 -- kept all requests in queues (too slow / too much latency overhead)

We would like to illustrate why it was a challenge to keep the overhead low.
For an Infiniband network, the latency for a small operation is on the order of
2 microseconds. That might sound like a lot, but it's fairly easy to take 2
microseconds in software (these are performance estimates):

  - 1 malloc call
  - 20 processor synchronizations
  - 20 cache lines loaded from RAM
  - 2000 instructions executed.

These numbers influenced our design:
  - we start a nonblocking GET as soon as we know the page is not in the cache
  - we pre-allocate all memory used by the cache
  - we create 1 cache per pthread (to avoid processor synchronization)
  - we did our best to minimize the amount of memory read/written when
    processing a put or a get.


Here's how pending requests and acquire barriers are handled.  We assign each
operation a sequence number using a thread-local counter. We record minimum and
maximum sequence numbers in cache entries. Each task has in task-local storage
the sequence number of the last acquire barrier. If a task performs a GET but
finds a cache entry with a minimum sequence number before its last acquire
barrier, it must invalidate that cache line and do a new GET.

Lastly, since the implementation uses thread-local storage for the cache, it
requires that tasks not move between threads. Tasks could move between threads
if we had a way to notify the cache that they were about to do so (in which
case the cache would issue a release barrier in the old thread and an acquire
barrier in the new thread). Another alternative would be to mark tasks with
ongoing operations in the cache as not movable to other threads. A third option
would be to only move tasks between threads in situations that create full
barriers anyway; notably a full barrier occurs on task start and sync variable
use.

 */

// ASSUMES THAT TASKS DO NOT MIGRATE BETWEEN PTHREADS
// because:
// 1) GASNet handles are only valid for a specific pthread
// 2) want to avoid synchronization on the cache data structures
//    but don't want to have 1 per task.
//
// If (1) is resolved, we could also weaken (2) by detecting
// when task-local storage != thread-local storage.
//
// See chapel-developers thread "migrating tasks" from 9/25/2013.
// FIFO: never moves a task from one pthread to another
// Qthreads workaround: QT_NUM_WORKERS_PER_SHEPHERD=1
//   (on 9/26/2013 Dylan mentioned perhaps adding 'pin to worker')

#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-comm-diags.h"
#include "chpl-tasks.h"
#include "chpl-mem.h"
#include "chpl-atomics.h"
#include "chpl-thread-local-storage.h" // CHPL_TLS_DECL etc
#include "chpl-cache.h"
#include "chpl-comm-strd-xfer.h"
#include "chpl-linefile-support.h"
#include "sys.h" // sys_page_size()
#include "chpl-comm-compiler-macros.h"
#include "chpl-comm-no-warning-macros.h" // No warnings for chpl_comm_get etc.


#include <string.h> // memcpy, memset, etc.
#include <assert.h>


#ifdef HAS_CHPL_CACHE_FNS

// When testing changes to the cache for correctness, it is a good practice
// to run tests with these two settings enabled.
//
// VERIFY will cause cache structure validiation to be run after each
// operation.
//
// EXTRA_YIELDS will call chpl_task_yield after comm layer calls
// that could yield.
#define VERIFY 0
#define EXTRA_YIELDS 0

// How many pending operations can we have at once?
#define MAX_PENDING 32

// CACHEPAGE_BITS
// Controls the cache page size - the cache manages items of this many bytes
// but also includes facilities for partial pages (valid and dirty bits).
//
// Reasonable values for CACHEPAGE_BITS are between 6 and 12
// (64 bytes and 4k bytes. CACHEPAGE_BITS should not be larger than the
// page size) and it must currently be even.
// Here we set it to 1k bytes (ie 2^10).
#define CACHEPAGE_BITS 10
#define CACHEPAGE_SIZE (1 << CACHEPAGE_BITS)
#define CACHEPAGE_MASK (CACHEPAGE_SIZE-1)

// CACHELINE_BITS
// Controls the cache line size - that is, the minimum number of bytes
// that are fetched for any 'get' operation.
//
// Reasonable values for CACHELINE_BITS are between 6 and CACHEPAGE_BITS.
// Here we set it to 64 bytes (ie 2^6)
#define CACHELINE_BITS 6
#define CACHELINE_SIZE (1 << CACHELINE_BITS)
#define CACHELINE_MASK (CACHELINE_SIZE-1)

// What type can store the number of cache lines in a cache page?
typedef int8_t line_per_page_t;
// What type for a number of lines to read ahead?
typedef int16_t readahead_distance_t;

// used to compress top_index_list / bottom_index arrays
// the entry pointer is entry_base + idx*sizeof(entry type)
typedef int16_t entry_id_t;

// When prefetching, what is the maximum number of pages
// we are willing to prefetch? This is also the maximum
// readahead window size for sequential access.
#define MAX_PAGES_PER_PREFETCH 2

// Should we enable sequential readahead?
// For sequential access If we're reading
#define ENABLE_READAHEAD 1
#define ENABLE_READAHEAD_TRIGGER_WITHIN_PAGE 1

#define ENABLE_READAHEAD_TRIGGER_SEQUENTIAL 0

#define MAX_SEQUENTIAL_READAHEAD_BYTES (MAX_PAGES_PER_PREFETCH*CACHEPAGE_SIZE)

// These defines can enable different kinds of debugging output.

//#define TIME

//#define TRACE_YIELDS
//#define TRACE_READAHEAD
//#define TRACE_NB
//#define TRACE

//#define TRACE_FENCES
//#define DEBUG
//#define DUMP
//#define INFO

#ifdef TIME

// For timing printouts.
#include <time.h>
static void print_time(void)
{
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  printf("%li.%09li ", t.tv_sec, t.tv_nsec);
}

static long time_duration(const struct timespec* t1, const struct timespec* t2)
{
  long duration = 1000000000*(t2->tv_sec-t1->tv_sec) + t2->tv_nsec - t1->tv_nsec;
  return duration;
}

#define TIME_PRINT(x) do { print_time(); printf x; } while(0);
#else
#define TIME_PRINT(x) do {} while(0);
#endif

#ifdef TRACE_YIELDS
#define TRACE_YIELD_PRINT(x) printf x
#else
#define TRACE_YIELD_PRINT(x) do {} while(0)
#endif

#ifdef TRACE_READAHEAD
#define TRACE_READAHEAD_PRINT(x) printf x
#else
#define TRACE_READAHEAD_PRINT(x) do {} while(0)
#endif

#ifdef TRACE_NB
#define TRACE_NB_PRINT(x) printf x
#else
#define TRACE_NB_PRINT(x) do {} while(0)
#endif

#ifdef TRACE
#define TRACE_PRINT(x) printf x
#else
#define TRACE_PRINT(x) do {} while(0)
#endif

#ifdef TRACE_FENCES
#define TRACE_FENCE_PRINT(x) printf x
#else
#define TRACE_FENCE_PRINT(x) do {} while(0)
#endif


#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x) do {} while(0)
#endif

#ifdef INFO
#define INFO_PRINT(x) printf x
#else
#define INFO_PRINT(x) do {} while(0)
#endif


// custom assert with stack trace
#if 0
#undef assert

#ifdef NDEBUG
#define assert(ignore)((void) 0)
#else
#define assert(x)                                                 \
  do {                                                            \
    if (!x) {                                                     \
      char* stack = NULL;                                         \
      stack = chpl_stack_unwind_to_string('\n');                  \
      if (stack != NULL) {                                        \
        fprintf(stderr, "assertion failed. Trace: %s", stack);    \
      }                                                           \
      __assert_fail(x, __FILE__, __LINE__, __func__);             \
  } while(0)
#endif
#endif

// ----------  SUPPORT FUNCTIONS
#include "chpl-cache-support.c"




// Forward Declarations.
struct rdcache_s;


//////////////// REMOTE DATA CACHE IMPLEMENTATION ////////////////////

#define TABLE_BITS 9
#define TABLE_SIZE (1 << TABLE_BITS)

// How many uint64_t words do we need to create a bitmask for CACHEPAGE_SIZE?
// Divide # bytes in cache by 64, rounding up.
#define CACHEPAGE_BITMASK_WORDS ((CACHEPAGE_SIZE+63)/64)

// How many cache lines per cache page?
#define CACHE_LINES_PER_PAGE (CACHEPAGE_SIZE/CACHELINE_SIZE)

// How many uint64_t words do we need to create a bitmask for CACHE_LINES_PER_PAGE
// ie, a mask recording a bit per cache line?
#define CACHE_LINES_PER_PAGE_BITMASK_WORDS (((CACHEPAGE_SIZE/CACHELINE_SIZE)+63)/64)

// Storing a remote address (node number is separate).
typedef uintptr_t raddr_t;

// This type contains the needed entry information to store in a lookup table.
// 16 bytes.
struct cache_table_entry_s {
  raddr_t raddr;   // always aligned to CACHEPAGE_SIZE
  c_nodeid_t node;
  uint32_t offset; // Add this number of bytes to cache base pointer
                   // to get pointer to struct cache_entry_s.
                   // Last bit stolen to indicate if more entries in list.
                   // 0 means no entry here.
};

// Similar to cache_table_entry_s but this type implements linked list
// chaining among entries in the case of too many collisions.
struct cache_list_entry_s {
  raddr_t raddr;   // always aligned to CACHEPAGE_SIZE
  c_nodeid_t node;
  struct cache_list_entry_s* next;
};

// Table entry. Intended to be 64 bytes.
#define TABLE_ENTRIES_PER_SLOT 4
struct cache_table_slot_s {
  struct cache_table_entry_s m[TABLE_ENTRIES_PER_SLOT];
};

// List for storing free pages. ~16 bytes/page
// TODO: could convert many of the next sections to use cache base-relative
// addressing and as a result save about 1/2 the space.
struct page_list_s {
  struct page_list_s* next;
  unsigned char* page;
};

// this data structure manages dirty pages. It is allocated separately
// from cache entries, so the majority of pages (which are clean) don't
// have the big bitmap. ~150 bytes/dirty entry.
struct dirty_entry_s {
  // linked list of currently dirty pages
  struct dirty_entry_s* next;
  struct dirty_entry_s* prev;
  // which cache entry are we talking about here?
  struct cache_entry_s* entry;
  // Which of the page's bytes are dirty?
  uint64_t dirty[CACHEPAGE_BITMASK_WORDS]; // ie we need to create a put for these bytes
};

#define QUEUE_FREE 0
#define QUEUE_AIN 1
#define QUEUE_AOUT 2
#define QUEUE_AM 3

static inline
raddr_t raddr_max(raddr_t a, raddr_t b)
{
    return (a>b)?a:b;
}
static inline
raddr_t raddr_min(raddr_t a, raddr_t b)
{
    return (a<b)?a:b;
}

// This entry stores the main information for the cache.
// ~88 bytes. TODO: can we shrink it to 64 bytes?
struct cache_entry_s {
  struct cache_list_entry_s base; // contains raddr, node, next offset
  // Queue information. This entry could be in Ain, Aout, or Am queues.
  int queue;

  // Since e.g. with ugni, a comm event can cause the implementation
  // to switch tasks, only allow one task at a time to manipulate
  // an entry. This does not need to be set if the entry is used
  // in code that cannot yield.
  // Set it to the task_local pointer for the task reserving the entry.
  chpl_cache_taskPrvData_t* entryReservedByTask;

  // Readahead information.
  readahead_distance_t readahead_skip;
  readahead_distance_t readahead_len; // == 0 if this page doesn't trigger readahead.
  // These are the queue links. Am is LRU but Ain and Aout are FIFO
  struct cache_entry_s* next; // next entry in Ain/Aout/Am
  struct cache_entry_s* prev; // previous entry in An/Aout/Am
  // Pointer to the cached data.
  // This refers to CACHEPAGE_SIZE bytes of memory.
  unsigned char* page;
  // Which of the cache lines have we done 'get's for?
  uint64_t valid_lines[CACHE_LINES_PER_PAGE_BITMASK_WORDS];
  // dirty info if this cache page is dirty, NULL otherwise.
  struct dirty_entry_s* dirty;
  // What is the minimum sequence number stored in this cache entry?
  // If we ran an acquire fence, we need to check that nothing in a cache line is too old.
  cache_seqn_t min_sequence_number;
  // If there are pending puts or prefetches for this cache entry, what is
  // the maximum sequence number of a pending operation?
  // If we run a release fence, we need to make sure that we have
  // completed all pending puts up to and including this sequence number.
  cache_seqn_t max_put_sequence_number;
  // If we try to read data from this cache entry, we need to make sure that
  // we have completed all pending prefetches before the lines are considered valid.
  cache_seqn_t max_prefetch_sequence_number;
};

// Note skip/len are in line numbers, NOT byte offsets!
static int check_valid_lines(uint64_t* valid, uintptr_t skip, uintptr_t len)
{
  return all_set_for_skip_len(valid, skip, len, CACHE_LINES_PER_PAGE_BITMASK_WORDS);
}

// Note skip/len are in line numbers, NOT byte offsets!
static int any_valid_lines(uint64_t* valid, uintptr_t skip, uintptr_t len)
{
  return any_set_for_skip_len(valid, skip, len, CACHE_LINES_PER_PAGE_BITMASK_WORDS);
}
// Note skip/len are in line numbers, NOT byte offsets!
static int set_valid_lines(uint64_t* valid, uintptr_t skip, uintptr_t len)
{
  set_valids_for_skip_len(valid,skip,len,CACHE_LINES_PER_PAGE_BITMASK_WORDS);
  return 1;
}
// Note skip/len are in line numbers, NOT byte offsets!
static void unset_valid_lines(uint64_t* valid, uintptr_t skip, uintptr_t len)
{
  uint64_t myvalid[CACHE_LINES_PER_PAGE_BITMASK_WORDS];
  unset_valids_for_skip_len(valid, myvalid, skip, len, CACHE_LINES_PER_PAGE_BITMASK_WORDS);
}

struct rdcache_s {
  // A 2Q cache.
  // See "2Q: A Low Overhead High Performance Buffer Management
  //      Replacement Algorithm"
  //    by Theodore Johnson and Dennis Sasha, Proc 20th VLDB conference, 1994.

  // The next request number -- there is currently no request or cache
  // element with this sequence number.
  cache_seqn_t next_request_number;
  // request number for the last completed request.
  cache_seqn_t completed_request_number;

  // Keep track of the raddr of the last cache miss read page in order
  // to enable sequential readahead.
  c_nodeid_t last_cache_miss_read_node;
  raddr_t last_cache_miss_read_addr;

  // Used with the lookup table. This is the number of bits
  // for the number of table slots.
  int table_bits;

  // The variable names Ain Aout and Am come from the 2Q paper

  // Ain is a FIFO queue storing entries initially as they go into
  // the cache. When an entry falls out of Ain, a record of that entry
  // is stored in Aout (without any data - ie not storing the cache line
  // itself, just the address). Entries are retired from Aout without event.
  // Am is a LRU queue consisting entirely of entries that have been accessed
  // again over time. When a cache miss has a record in Aout, instead of
  // storing that cache line in Ain, we store it in Am since it was needed
  // again once (and so probably has long-term value).
  //
  // In order to quickly identify the relevant cache entries by address,
  // besides being stored in a queue (FIFO in the case of Ain and Aout,
  // and LRU in the case of Am), each cache entry is also stored in the
  // 'pointer tree'. The 'pointer tree' is really a set of two hashtables
  // each using a simple hash function consisting of portions of a 64-bit
  // address.
  //
  // In order to keep allocator activity down, the leaf elements of the
  // tree (ie the cache entries themselves) are stored in circular
  // bounded buffers for Ain and Aout. For Am, they are allocated
  // (because LRU does not allow us to predict ahead of time in what order
  //  they will need to be deallocated). However, even with Am, in most cases
  // we are able to reuse a cache entry instead of freeing and reallocating.
  //
  // Ain and Aout stores entries and pending operations and newly cached data
  // and looks like this:
  //  <--                  <--                   <--                       <--
  //  tail              complete                 start                     head
  //  "first entry"                                                "last entry"
  //  | completed requests | in-progress requests | write/prefetch requests |
  //                                                not yet started
  //

  int max_pages;
  int max_entries;

  // Free pages
  struct page_list_s* free_pages_head; // singly-linked list
  // Free page list entries (all page pointers should be NULL)
  struct page_list_s* free_page_list_entries_head; // singly-linked list
  // Free nodes
  struct cache_list_entry_s* free_entries_head; // singly-linked list

  // Ain, a FIFO queue of entries not seen before
  unsigned int ain_max; // maximum; corresponds to Kin
  unsigned int ain_current; // current length of ain list
  struct cache_entry_s *ain_head;
  struct cache_entry_s *ain_tail;

  // Aout, a FIFO queue of entries fallen off of Ain
  // note entries in Aout should have entry->addr==NULL
  unsigned int aout_max; // maximum; corresponds to Kout
  unsigned int aout_current; // current length of aout list
  struct cache_entry_s *aout_head;
  struct cache_entry_s *aout_tail;

  // Am represents the LRU queue, in which we will store cached data that was
  // fetched again while being in Aout (after falling off of the Ain queue)
  unsigned int am_current; // current length of am list
  struct cache_entry_s* am_lru_head;
  struct cache_entry_s* am_lru_tail;

  // List of dirty pages (for write-combining)
  int num_dirty_pages;
  struct dirty_entry_s *dirty_lru_head;
  struct dirty_entry_s *dirty_lru_tail;

  // chpl-comm handles for pending operations
  // After requests are started, they will be stored here..
  unsigned int pending_len; // must be >0 and a power of 2
  int pending_first_entry;
  int pending_last_entry;
  chpl_comm_nb_handle_t *pending;
  cache_seqn_t *pending_sequence_numbers;

  // Lookup table
  __attribute__ ((aligned (64)))
  struct cache_table_slot_s table[];
};

static void validate_cache(struct rdcache_s* tree,
                           chpl_cache_taskPrvData_t* task_local);

static inline
uint32_t clear_offset_stolen_bits(uint32_t offset_with_bits) {
  uint32_t no_last_bit;
  no_last_bit = 1;
  no_last_bit = ~no_last_bit;
  return offset_with_bits & no_last_bit;
}

static inline
struct cache_entry_s* offset_to_entry(struct rdcache_s* tree,
                                      uint32_t offset_with_bits) {
  uint32_t offset;
  unsigned char* base;
  unsigned char* ptr;
  struct cache_entry_s* ret;

  offset = clear_offset_stolen_bits(offset_with_bits);

  base = (unsigned char*) tree;
  ptr = base + offset;
  ret = (struct cache_entry_s*) ptr;

  return (offset!=0)?ret:NULL;
}

static inline
uint32_t entry_to_offset(struct rdcache_s* tree, struct cache_entry_s* entry) {
  uint32_t offset;
  unsigned char* base;
  unsigned char* ptr;

  base = (unsigned char*)tree;
  ptr = (unsigned char*) entry;
  offset = ptr - base;

  return (entry!=NULL)?offset:0;
}

static void check_line_aligned(void* ptr)
{
  assert(((intptr_t) ptr) % 64 == 0);
}

// aka create_cache
static
struct rdcache_s* cache_create(void) {
  struct rdcache_s* c;
  int cache_pages;
  int ain_pages, aout_pages;
  int dirty_pages;
  int i;
  int n_entries;
  int table_entries;
  int table_slots;
  int table_bits;
  struct page_list_s* page_list_entries = NULL;
  struct cache_entry_s *entries = NULL;
  struct dirty_entry_s *dirty_nodes = NULL;
  uintptr_t offset;

  size_t total_size = 0;
  size_t allocated_size = 0;
  unsigned int pending_len = MAX_PENDING;
  unsigned char* buffer;
  unsigned char* pages;

  // This used to grow based on the number of locales, but that
  // would mean increasing memory usage per node, which isn't acceptable.
  cache_pages = 1024;

  ain_pages = cache_pages / 4; // 2Q: "Kin should be 25% of page slots"
                               // but here we set it smaller so that
                               // we can have #top_entries == ain
                               // and still have limited overhead.
  aout_pages = cache_pages / 2; // 2Q: "Kout should hold identifiers for as
                                // many pages as would fit in 50% of the
                                // buffer"
  // How many pages can be dirty at once?
  dirty_pages = 16 + cache_pages / 64;

  // How many cache entries do we need?
  n_entries = cache_pages + aout_pages;

  // Lookup table size (note each slot is 4 entries, so this is 4x slots)
  // table_entries and table_slots must both be a power of 2
  table_entries = 2 * cache_pages;
  assert((table_entries & (table_entries - 1)) == 0); // check it's a power of 2
  table_slots = table_entries * sizeof(struct cache_table_entry_s) /
                                sizeof(struct cache_table_slot_s);
  assert((table_slots & (table_slots - 1)) == 0); // check it's a power of 2
  // compute the number of bits in table_slots so that
  // table_slots == 1 << table_bits
  table_bits = 0;
  while ( (1 << table_bits) < table_slots ) {
    table_bits++;
  }
  assert( (1 << table_bits) == table_slots );

  total_size += sizeof(struct rdcache_s);
  total_size += table_slots * sizeof(struct cache_table_slot_s);
  total_size += sizeof(struct page_list_s) * cache_pages;
  total_size += sizeof(struct cache_entry_s) * n_entries;
  total_size += sizeof(struct dirty_entry_s) * dirty_pages;
  total_size += sizeof(chpl_comm_nb_handle_t) * pending_len;
  total_size += sizeof(cache_seqn_t) * pending_len;
  // We allocate an extra page for alignment
  total_size += CACHEPAGE_SIZE + CACHEPAGE_SIZE * cache_pages;

  // Now, allocate it all in one go.
  buffer = chpl_memalign(64, total_size);
  allocated_size = total_size;

  // Now divvy up the portions...
  total_size = 0;
  c = (struct rdcache_s*) (buffer + total_size);
  total_size += sizeof(struct rdcache_s);
  total_size += table_slots * sizeof(struct cache_table_slot_s);
  check_line_aligned(c);
  check_line_aligned(c->table);

  // page list entries
  page_list_entries = (struct page_list_s*) (buffer + total_size);
  total_size += sizeof(struct page_list_s) * cache_pages;
  // entries
  entries = (struct cache_entry_s*) (buffer + total_size);
  total_size += sizeof(struct cache_entry_s) * n_entries;
  assert( (((uintptr_t) entries) & 1) == 0 ); // problems if stolen bit set
  // dirty entries
  dirty_nodes = (struct dirty_entry_s*) (buffer + total_size);
  total_size += sizeof(struct dirty_entry_s) * dirty_pages;
  // and the pending data area
  c->pending = (chpl_comm_nb_handle_t*) (buffer + total_size);
  total_size += sizeof(chpl_comm_nb_handle_t) * pending_len;
  // and the pending sequence numbers
  c->pending_sequence_numbers = (cache_seqn_t*) (buffer + total_size);
  total_size += sizeof(cache_seqn_t) * pending_len;
  // Now, page-align the page allocations.
  offset = (((uintptr_t) buffer) + total_size) % CACHEPAGE_SIZE;
  if( offset != 0 ) offset = CACHEPAGE_SIZE - offset;
  total_size += offset;
  assert( ( (uintptr_t) (buffer + total_size) ) % CACHEPAGE_SIZE == 0 );
  // and finally allocate the pages
  pages = buffer + total_size;
  total_size += CACHEPAGE_SIZE * cache_pages;

  if (total_size > allocated_size) {
    chpl_internal_error("cache_create() failed");
  }

  // Now fill in everything else.
  c->next_request_number = 1;
  c->completed_request_number = 0;

  c->last_cache_miss_read_node = -1;
  c->last_cache_miss_read_addr = 0;

  c->max_pages = cache_pages;
  c->max_entries = n_entries;

  // Set up free_pages as a linked list of page list entries
  // pointing to the free pages.
  c->free_pages_head = &page_list_entries[0];
  for( i = 0; i < cache_pages; i++ ) {
    struct page_list_s* next;
    if( i + 1 < cache_pages ) next = &page_list_entries[i+1];
    else next = NULL;
    page_list_entries[i].next = next;
    page_list_entries[i].page = pages + i * CACHEPAGE_SIZE;
  }

  // Nothing is in free_page_list_entries initially.
  c->free_page_list_entries_head = NULL;

  // Set up free_entries as a linked list of free entries.
  c->free_entries_head = &entries[0].base;
  for( i = 0; i < n_entries; i++ ) {
    struct cache_list_entry_s* next;
    if( i + 1 < n_entries ) next = &entries[i+1].base;
    else next = NULL;
    entries[i].base.next = next;
    entries[i].queue = QUEUE_FREE;
    entries[i].entryReservedByTask = NULL;
  }


  c->ain_max = ain_pages;
  c->ain_current = 0;
  c->ain_head = NULL;
  c->ain_tail = NULL;


  c->aout_max = aout_pages;
  c->aout_current = 0;
  c->aout_head = NULL;
  c->aout_tail = NULL;

  c->am_current = 0;
  c->am_lru_head = NULL;
  c->am_lru_tail = NULL;

  c->num_dirty_pages = 0;
  c->dirty_lru_head = NULL;
  c->dirty_lru_tail = NULL;
  // set up dirty_lru as a linked list of dirty entries
  c->dirty_lru_head = &dirty_nodes[0];
  for( i = 0; i < dirty_pages; i++ ) {
    struct dirty_entry_s* next;
    struct dirty_entry_s* prev;
    if( i + 1 < dirty_pages ) next = &dirty_nodes[i+1];
    else next = NULL;
    if( i > 0 ) prev = &dirty_nodes[i-1];
    else prev = NULL;
    dirty_nodes[i].next = next;
    dirty_nodes[i].prev = prev;
    dirty_nodes[i].entry = NULL;
  }
  c->dirty_lru_tail = &dirty_nodes[dirty_pages-1];

  c->pending_len = MAX_PENDING;
  c->pending_first_entry = -1;
  c->pending_last_entry = -1;
  // already set c->pending to allocated region
  // already set c->pending_sequence_numbers to allocated region

  // fill in the lookup table
  c->table_bits = table_bits;
  for( i = 0; i < table_slots; i++ ) {
    for (int j = 0; j < TABLE_ENTRIES_PER_SLOT; j++) {
      c->table[i].m[j].raddr = 0;
      c->table[i].m[j].node = 0;
      c->table[i].m[j].offset = 0;
    }
  }

  if( VERIFY ) validate_cache(c, NULL);

  return c;
}

static
void cache_destroy(struct rdcache_s *cache) {
  chpl_free(cache);
}


static
void cache_entry_print(struct rdcache_s* cache, struct cache_entry_s* entry, const char* prefix, int print_data)
{
  int i,j,line;
  int valid;
  int valid_line;
  int dirty;
  printf("%scache entry id %i node %i next %i\n",
         prefix,
         (int) entry_to_offset(cache, entry),
         entry->base.node,
         (int) entry_to_offset(cache, (struct cache_entry_s*)entry->base.next));
  printf("%sraddr %p queue %i readahead_skip %i readahead_len %i next %p prev %p page %p\n",
         prefix, (void*) entry->base.raddr, entry->queue, (int) entry->readahead_skip, (int) entry->readahead_len, entry->next, entry->prev, entry->page);
  printf("%smin_seq %d max_put_seq %d max_prefetch_seq %d\n", prefix,
         (int) entry->min_sequence_number,
         (int) entry->max_put_sequence_number,
         (int) entry->max_prefetch_sequence_number);
  if( print_data && entry->page ) {
    printf("%svalid lines: %llx\n", prefix, (long long int) entry->valid_lines[0]);
    printf("%sdata:", prefix);


    for( line = 0; line < CACHE_LINES_PER_PAGE; line++ ) {
      valid_line = check_valid_lines(entry->valid_lines, line, 1);
      for( j = 0; j < CACHELINE_SIZE; j++ ) {
        i = CACHELINE_SIZE * line + j;
        if( (j % 32) == 0) {
          if( valid_line ) printf("\n%s v %04x ", prefix, i);
          else printf("\n%s - %04x ", prefix, i);
        }
        valid = valid_line;
        if( entry->dirty ) {
          valid |= any_set_for_skip_len(entry->dirty->dirty, i, 1, CACHEPAGE_BITMASK_WORDS);
        }
        if( valid ) {
          printf("%02x", entry->page[i]);
        } else {
          printf("--");
        }
      }
      for( j = 0; j < CACHELINE_SIZE; j++ ) {
        i = CACHELINE_SIZE * line + j;
        if( (j % 32) == 0) {
          printf("\n%s d?%04x ", prefix, i);
        }
        dirty = 0;
        if( entry->dirty ) {
          dirty = any_set_for_skip_len(entry->dirty->dirty, i, 1, CACHEPAGE_BITMASK_WORDS);
        }
        if( dirty ) {
          printf("DD");
        } else {
          printf("  ");
        }
      }
    }
    printf("\n");
  }
}

static
void rdcache_print(struct rdcache_s* cache) {
  struct cache_entry_s* entry;

  if( ! cache ) return;

  printf("  next_request_number %d\n", (int) cache->next_request_number);
  printf("  completed_request_number %d\n", (int) cache->completed_request_number);
  printf("  Ain:\n");
  for( entry = cache->ain_head; entry; entry = entry->next ) {
    cache_entry_print(cache, entry, "    ain ", 1);
  }
  printf("  Aout:\n");
  for( entry = cache->aout_head; entry; entry = entry->next ) {
    cache_entry_print(cache, entry, "   aout ", 1);
  }
  printf("  Am:\n");
  for( entry = cache->am_lru_head; entry; entry = entry->next ) {
    cache_entry_print(cache, entry, "     am ", 1);
  }

  fflush(stdout);
}

static inline
uint64_t hash_raddr(raddr_t raddr, int32_t node) {
  uint64_t val = raddr >> CACHEPAGE_BITS;
  val ^= node;
  val ^= raddr >> (27 + CACHEPAGE_BITS);
  return val;
}

static inline
int hash_to_slot(uint64_t hash, unsigned int table_bits) {
  unsigned int h = hash;
  unsigned int table_mask = (1 << table_bits) - 1;
  return (int) (h & table_mask);
}
static inline
int hash_to_entry(uint64_t hash, unsigned int table_bits) {
  unsigned int h = hash >> table_bits;
  return (int) (h % TABLE_ENTRIES_PER_SLOT);
}

// Looks up raddr/node in the table and linked lists.
// Returns an entry that is found to match.
// If an entry is found, return it and:
//  *prev_list will store the previous list entry (from entry.base)
//  *prev_table will store the top level table entry ultimately used
// If an entry is not found, return NULL and leave prev_table/prev_list NULL.
static
struct cache_entry_s* lookup_entry_prev(struct rdcache_s* tree,
                                        const int32_t node, const raddr_t raddr,
                                        struct cache_table_entry_s** prev_table,
                                        struct cache_entry_s** prev_list) {

  struct cache_table_slot_s* slot;
  uint64_t hash;
  int hash_slot;
  int hash_entry;
  struct cache_entry_s* prev[TABLE_ENTRIES_PER_SLOT];
  struct cache_entry_s* cur[TABLE_ENTRIES_PER_SLOT];

  hash = hash_raddr(raddr, node);
  hash_slot = hash_to_slot(hash, tree->table_bits);
  hash_entry = hash_to_entry(hash, tree->table_bits);

  slot = &tree->table[hash_slot];

  // Identify any entries in the main table that match and populate cur.
  // Search starting at the preferred entry.
  for (int j = 0; j < TABLE_ENTRIES_PER_SLOT; j++) {
    struct cache_table_entry_s* table_entry;
    struct cache_entry_s* cache_entry;
    int idx = (hash_entry+j) % TABLE_ENTRIES_PER_SLOT;
    table_entry = &slot->m[idx];
    cache_entry = offset_to_entry(tree, table_entry->offset);
    if (raddr == table_entry->raddr &&
        node == table_entry->node &&
        0 != table_entry->offset) {
      *prev_table = table_entry;
      *prev_list = NULL;
      return cache_entry;
    }
    // Track the lists we need to traverse
    if ((table_entry->offset & 1) == 1)
      cur[idx] = cache_entry;
    else
      cur[idx] = NULL;
  }

  // Advance all the lists by one entry since the raddr/node
  // in the entry is the same as the one in the lookup table.
  for (int idx = 0; idx < TABLE_ENTRIES_PER_SLOT; idx++) {
    prev[idx] = cur[idx];
    if (cur[idx] != NULL)
      cur[idx] = (struct cache_entry_s*) cur[idx]->base.next;
  }

  // Traverse the lists
  while (1) {
    int any_lists = 0;
    // Check each cur list for a match and note if all are NULL.
    for (int idx = 0; idx < TABLE_ENTRIES_PER_SLOT; idx++) {
      if (cur[idx] != NULL) {
        any_lists = 1;
        if (raddr == cur[idx]->base.raddr && node == cur[idx]->base.node) {
          *prev_table = &slot->m[idx];
          *prev_list = prev[idx];
          return cur[idx];
        }
      }
    }

    // Stop if all cur lists are NULL
    if (any_lists == 0)
      break;

    // Advance each list to the next
    for (int idx = 0; idx < TABLE_ENTRIES_PER_SLOT; idx++) {
      if (cur[idx] != NULL) {
        prev[idx] = cur[idx];
        cur[idx] = (struct cache_entry_s*) cur[idx]->base.next;
      }
    }
  }

  // nothing found.
  *prev_table = NULL;
  *prev_list = NULL;
  return NULL;
}

static inline
struct cache_entry_s* lookup_entry(struct rdcache_s* tree,
                                   int32_t node, raddr_t raddr) {

  struct cache_table_entry_s* prev_table;
  struct cache_entry_s* prev_list;
  return lookup_entry_prev(tree, node, raddr, &prev_table, &prev_list);
}

// When we know the entry is not in the tree, use this function
// to find out the table location in which to add it.
static
struct cache_table_entry_s* lookup_entry_adding(struct rdcache_s* tree,
                                                int32_t node, raddr_t raddr) {

  struct cache_table_slot_s* slot;
  uint64_t hash;
  int hash_slot;
  int hash_entry;
  int first_empty_entry;
  int use_entry;

  hash = hash_raddr(raddr, node);
  hash_slot = hash_to_slot(hash, tree->table_bits);
  hash_entry = hash_to_entry(hash, tree->table_bits);

  slot = &tree->table[hash_slot];

  // Find an empty entry, if one exists.
  first_empty_entry = -1;
  for (int j = 0; j < TABLE_ENTRIES_PER_SLOT; j++) {
    struct cache_table_entry_s* table_entry;
    // Visit entries starting from hash_entry to spread out
    // where new entries go.
    int idx = (hash_entry+j) % TABLE_ENTRIES_PER_SLOT;
    table_entry = &slot->m[idx];
    if (table_entry->offset == 0 && first_empty_entry == -1) {
      first_empty_entry = idx;
      break; // this break could be removed, if it helps
    }
  }

  if (first_empty_entry == -1)
    use_entry = hash_entry; // no empty entries, so use preferred one
  else
    use_entry = first_empty_entry;

  return &slot->m[use_entry];
}

// Removes 'element' from the tree.
// Does not wait for any operations to complete or free any cache entries
static
void tree_remove(struct rdcache_s* tree, struct cache_entry_s* element)
{
  struct cache_table_entry_s* prev_table;
  struct cache_entry_s* prev_list;
  struct cache_entry_s* entry;
  int32_t node;
  raddr_t raddr;
  struct cache_entry_s* next;

  DEBUG_PRINT(("%d: Removing %p element %p\n", chpl_nodeID,
               (void*) element->raddr, element));

  raddr = element->base.raddr;
  node = element->base.node;
  next = (struct cache_entry_s *) element->base.next;
  entry = lookup_entry_prev(tree, node, raddr, &prev_table, &prev_list);

  // entry should be found in the tree
  // (redundancy here avoids warning about unused variable)
  if (entry == NULL) assert(entry);
  // prev should be set for something in the tree.
  // prev will need adjustment.
  assert(prev_table);

  if (prev_list) {
    // our entry was pointed to by another entry
    if (prev_table &&
        prev_list->base.raddr == prev_table->raddr &&
        prev_list->base.node == prev_table->node &&
        next == NULL) {
      // e.g., when removing X
      //      table -> entry -> X (at end of list)
      // ->   table -> entry  and clear table entry's list bit
      prev_table->offset &= clear_offset_stolen_bits(prev_table->offset);
    }
    // otherwise, just update the offset of the previous list entry
    prev_list->base.next = (struct cache_list_entry_s*) next;
  } else if (prev_table) {
    // our entry was pointed to by the table
    if (VERIFY) {
      assert(prev_table->node == entry->base.node);
      assert(prev_table->raddr == entry->base.raddr);
      assert(clear_offset_stolen_bits(prev_table->offset) ==
             entry_to_offset(tree, element));
    }
    if (next) {
      // Store the next entry in the table
      // Can use the same slot since it was a collision.
      uint32_t next_offset;
      next_offset = entry_to_offset(tree, next);
      if (next->base.next)
        next_offset |= 1; // set list flag if next entry has an next
      prev_table->raddr = next->base.raddr;
      prev_table->node = next->base.node;
      prev_table->offset = next_offset;
    } else {
      // Clear out the table entry
      prev_table->raddr = 0;
      prev_table->node = 0;
      prev_table->offset = 0;
    }
  }
}


// flush_entry operations mask

// Apply the operation to the entire page (ignoring raddr/len)
#define FLUSH_DO_PAGE 1
// Start puts for dirty bits in the region
#define FLUSH_DO_CLEAR_DIRTY 2
// Flush pending operations that could overlap with the region
#define FLUSH_DO_PENDING 4
// Invalidate the page (if FLUSH_PAGE is also set) or the region
#define FLUSH_DO_INVALIDATE 8
// Evict the page. entry->page will be added to a free list.
#define FLUSH_DO_EVICT 16

// These are the normally-used combinations
#define FLUSH_EVICT (FLUSH_DO_PAGE|FLUSH_DO_CLEAR_DIRTY|FLUSH_DO_PENDING|FLUSH_DO_EVICT)
#define FLUSH_INVALIDATE_REGION (FLUSH_DO_CLEAR_DIRTY|FLUSH_DO_PENDING|FLUSH_DO_INVALIDATE)
#define FLUSH_INVALIDATE_PAGE (FLUSH_DO_PAGE|FLUSH_DO_CLEAR_DIRTY|FLUSH_DO_PENDING|FLUSH_DO_INVALIDATE)
#define FLUSH_PREPARE_PUT (FLUSH_DO_PENDING)
#define FLUSH_PREPARE_GET (FLUSH_DO_CLEAR_DIRTY|FLUSH_DO_PENDING)

static
void flush_entry(struct rdcache_s* cache,
                 chpl_cache_taskPrvData_t* task_local,
                 struct cache_entry_s* entry, int op,
                 raddr_t raddr, int32_t len_in);
static
int try_reserve_entry(struct rdcache_s* cache,
                      chpl_cache_taskPrvData_t* task_local,
                      struct cache_entry_s* entry);

static
void unreserve_entry(struct rdcache_s* cache,
                   chpl_cache_taskPrvData_t* task_local,
                   struct cache_entry_s* entry);


static
void aout_evict(struct rdcache_s* cache)
{
  struct cache_entry_s* z;
  struct cache_list_entry_s* entry;

  z = cache->aout_tail;

  if( !z ) return;

  assert(z->entryReservedByTask == NULL);

  // Remove the tail element from Aout
  DOUBLE_REMOVE_TAIL(cache, aout);
  cache->aout_current--;

  // Remove entry (which we are kicking off of Aout) from the tree
  tree_remove(cache, z);

  z->queue = QUEUE_FREE;

  // and store it on the free list.
  entry = &z->base;
  SINGLE_PUSH_HEAD(cache, entry, free_entries);
}

static
void ain_evict(struct rdcache_s* cache,
               chpl_cache_taskPrvData_t* task_local,
               int give_up_if_locked)
{

  while (1) {
    struct cache_entry_s* y = cache->ain_tail;

    if (y==NULL)
      return;

    assert(y->entryReservedByTask != task_local);
    // give up if locked
    if (give_up_if_locked && y->entryReservedByTask != NULL)
      return;

    if (!try_reserve_entry(cache, task_local, y)) {
      // could not "lock" entry y so try again
      TRACE_YIELD_PRINT(("%d: task %d cache %p yielding in ain_evict\n",
                        chpl_nodeID, (int) chpl_task_getId(), cache));

      chpl_task_yield();

      TRACE_YIELD_PRINT(("%d: task %d cache %p back in ain_evict\n",
                        chpl_nodeID, (int) chpl_task_getId(), cache));
      continue;
    }

    // "lock"ed entry y

#ifdef DEBUG
    DEBUG_PRINT(("Ain is evicting entry for raddr %p\n", (void*) y->raddr));
    cache_entry_print( y, " ain evict ", 1);
    rdcache_print(cache);
#endif

    // If the entry in Ain has any pending/dirty requests, we must
    // immediately wait for them to complete, before we modify the contents
    // of Ain in any way (or reuse the associated page).
    flush_entry(cache, task_local, y, FLUSH_EVICT, 0, CACHEPAGE_SIZE);
    assert(y->entryReservedByTask == task_local);
    assert(y->queue == QUEUE_AIN && y == cache->ain_tail);

    DOUBLE_REMOVE_TAIL(cache, ain);
    cache->ain_current--;

    y->queue = QUEUE_AOUT;

    // Since we are kicking entry off of Ain, we have to add it to Aout.
    DOUBLE_PUSH_HEAD(cache, y, aout);
    cache->aout_current++;

    // "unlock" entry y
    unreserve_entry(cache, task_local, y);

    if( cache->aout_current > cache->aout_max ) {
      // Remove the tail element from aout.
      aout_evict(cache);
    }
    return;
  }
}


static
void am_evict(struct rdcache_s *cache,
              chpl_cache_taskPrvData_t* task_local,
              int give_up_if_locked)
{

  while (1) {
    struct cache_entry_s* y = cache->am_lru_tail;
    struct cache_list_entry_s* entry;

    if (y==NULL)
      return;

    assert(y->entryReservedByTask != task_local);
    // give up if locked
    if (give_up_if_locked && y->entryReservedByTask != NULL)
      return;

    if (!try_reserve_entry(cache, task_local, y)) {
      // could not "lock" entry y so try again
      TRACE_YIELD_PRINT(("%d: task %d cache %p yielding in am_evict\n",
                        chpl_nodeID, (int) chpl_task_getId(), cache));

      chpl_task_yield();

      TRACE_YIELD_PRINT(("%d: task %d cache %p back in am_evict\n",
                        chpl_nodeID, (int) chpl_task_getId(), cache));
      continue;
    }

    // "lock"ed entry y

    // If the entry in Am has any pending/dirty requests, we must
    // immediately wait for them to complete, before we modify the contents
    // of Ain in any way (or reuse the associated page).
    flush_entry(cache, task_local, y, FLUSH_EVICT, 0, CACHEPAGE_SIZE);
    assert(y->entryReservedByTask == task_local);
    assert(y->queue == QUEUE_AM && y == cache->am_lru_tail);

    DOUBLE_REMOVE_TAIL(cache, am_lru);
    cache->am_current--;

    // Remove this entry in Am from the pointer tree.
    tree_remove(cache, y);

    y->queue = QUEUE_FREE;

    // Add y to the free list
    entry = &y->base;
    SINGLE_PUSH_HEAD(cache, entry, free_entries);

    // "unlock" entry y
    unreserve_entry(cache, task_local, y);
    return;
  }
}

static
void reclaim(struct rdcache_s* cache,
             chpl_cache_taskPrvData_t* task_local,
             int give_up_if_locked)
{
  // This is like 'reclaimfor' in the 2Q paper
  // if the number of elements in Ain > max
  if( cache->ain_current > cache->ain_max ) {
    // Page out the tail of Ain (and record it in Aout)
    // ain_evict will also evict from aout if necessary.
    ain_evict(cache, task_local, give_up_if_locked);
  } else {
    // otherwise
    // page out the tail of Am, call it Y
    // do not put it on Aout, as it hasn't been accessed recently.
    am_evict(cache, task_local, give_up_if_locked);
  }
}

static
struct cache_entry_s* allocate_entry(struct rdcache_s* cache)
{
  struct cache_entry_s* ret;

  // Make sure we have a free entry..
  assert(cache->free_entries_head != NULL); // caller's responsibility

  ret = (struct cache_entry_s*) cache->free_entries_head;

  // Remove it from the single-linked list.
  SINGLE_POP_HEAD(cache, free_entries);

  return ret;
}


// Make sure there is a free page on the free list,
// but don't evict a page with an ongoing operation..
static
void ensure_free_page(struct rdcache_s* cache,
                      chpl_cache_taskPrvData_t* task_local,
                      int give_up_if_locked)
{
  // If there are free page slots, then use a free page slot.
  if( cache->free_pages_head ) {
    return;
  }

  reclaim(cache, task_local, give_up_if_locked);

  if (!give_up_if_locked)
    assert( cache->free_pages_head && cache->free_pages_head->page );
}

static
unsigned char* allocate_page(struct rdcache_s* cache)
{
  struct page_list_s* page_list_entry;
  unsigned char* ret;

  assert(cache->free_pages_head != NULL); // caller's responsibility.

  page_list_entry = cache->free_pages_head;
  SINGLE_POP_HEAD(cache, free_pages);

  ret = page_list_entry->page;
  page_list_entry->page = NULL;
  SINGLE_PUSH_HEAD(cache, page_list_entry, free_page_list_entries);

  return ret;
}

static
void ensure_free_dirty(struct rdcache_s* cache,
                       chpl_cache_taskPrvData_t* task_local)
{
  assert(cache->dirty_lru_tail);

  while (1) {
    struct cache_entry_s* entry = cache->dirty_lru_tail->entry;
    if (entry == NULL)
      return;

    if (!try_reserve_entry(cache, task_local, entry)) {
      // could not "lock" entry so try again
      TRACE_YIELD_PRINT(("%d: task %d cache %p yielding in ensure_free_dirty\n",
                        chpl_nodeID, (int) chpl_task_getId(), cache));

      chpl_task_yield();

      TRACE_YIELD_PRINT(("%d: task %d cache %p back in ensure_free_dirty\n",
                        chpl_nodeID, (int) chpl_task_getId(), cache));
      continue;
    }

    // "lock"ed entry
    flush_entry(cache, task_local, entry,
              FLUSH_DO_PAGE|FLUSH_DO_CLEAR_DIRTY, 0, CACHEPAGE_SIZE);
    unreserve_entry(cache, task_local, entry);
  }
}


static
struct dirty_entry_s* allocate_dirty(struct rdcache_s* cache,
                                     chpl_cache_taskPrvData_t* task_local,
                                     struct cache_entry_s* for_entry)
{
  struct dirty_entry_s* dirty;

  // We never remove from dirty_lru, just reorder...
  assert( cache->dirty_lru_tail );

  if( cache->dirty_lru_tail->entry ) ensure_free_dirty(cache, task_local);

  dirty = cache->dirty_lru_tail;
  DOUBLE_REMOVE_TAIL(cache, dirty_lru);
  DOUBLE_PUSH_HEAD(cache, dirty, dirty_lru);
  cache->num_dirty_pages++;

  memset(dirty->dirty, 0, sizeof(uint64_t) * CACHEPAGE_BITMASK_WORDS);

  dirty->entry = for_entry;
  for_entry->dirty = dirty;

  return dirty;
}

static void use_dirty(struct rdcache_s* cache, struct dirty_entry_s* dirty)
{
  DOUBLE_REMOVE(cache, dirty, dirty_lru);
  DOUBLE_PUSH_HEAD(cache, dirty, dirty_lru);
}


static
void do_wait_for(struct rdcache_s* cache, cache_seqn_t sn);

static inline
void wait_for(struct rdcache_s* cache, cache_seqn_t sn)
{
  // Do nothing if we have already completed sn.
  if( sn <= cache->completed_request_number ) return;
  do_wait_for(cache, sn);
}

static
cache_seqn_t pending_push(struct rdcache_s* cache, chpl_comm_nb_handle_t handle)
{
  cache_seqn_t wait_sn;
  cache_seqn_t sn;
  int index;

  DEBUG_PRINT(("in pending_push nelements=%i/%i\n",
               fifo_circleb_count(cache->pending_first_entry, cache->pending_last_entry, cache->pending_len),
               cache->pending_len));

  while (fifo_circleb_isfull(cache->pending_first_entry,
                             cache->pending_last_entry,
                             cache->pending_len) ) {
    index = cache->pending_first_entry;
    wait_sn = cache->pending_sequence_numbers[index];
    wait_for(cache, wait_sn);
  }

  sn = cache->next_request_number;
  cache->next_request_number++;

  fifo_circleb_push(&cache->pending_first_entry, &cache->pending_last_entry, cache->pending_len);
  index = cache->pending_last_entry;
  cache->pending[index] = handle;
  cache->pending_sequence_numbers[index] = sn;

  TRACE_NB_PRINT(("%d: task %d pending_push added "
                  "pending[%i]=%p sn=%i completed=%i\n",
                  chpl_nodeID, (int)chpl_task_getId(),
                  index, (void*) handle, (int) sn,
                  (int) cache->completed_request_number));

  return sn;
}

#if VERIFY
static
int find_in_dirty(struct rdcache_s* tree, struct dirty_entry_s* entry)
{
  struct dirty_entry_s* head = tree->dirty_lru_head;
  struct dirty_entry_s* cur;
  for( cur = head; cur; cur = cur->next ) {
    if( cur == entry ) return 1;
  }
  return 0;
}

static
int find_in_queue(struct cache_entry_s* head, struct cache_entry_s* entry)
{
  struct cache_entry_s* cur;
  for( cur = head; cur; cur = cur->next ) {
    if( cur == entry ) return 1;
  }
  return 0;
}

static
int validate_queue(struct rdcache_s* tree,
                   chpl_cache_taskPrvData_t* task_local,
                   struct cache_entry_s* head,
                   struct cache_entry_s* tail,
                   int queue)
{
  struct cache_entry_s* cur;
  struct cache_entry_s* found;
  int forward_count = 0;
  int reverse_count = 0;
  // check:
  // -- doubly linked list integrity
  // -- everything in the tree unless queue==QUEUE_FREE

  for( cur = head; cur; cur = cur->next ) {
    if( cur->next ) assert(cur->next->prev == cur);

    // and check some additional properties. In tree?
    if( queue != QUEUE_FREE ) {
      found = lookup_entry(tree, cur->base.node, cur->base.raddr);
      assert(found == cur);
    }

    // Correct queue tag?
    assert(cur->queue == queue);

    // Ain and Am only have page?
    if( queue == QUEUE_AIN || queue == QUEUE_AM ) {
      assert(cur->page);
      if( cur->dirty ) {
        // Check that it's in the dirty list.
        assert(find_in_dirty(tree, cur->dirty));
      }
    } else {
      assert(! cur->page);
      assert(! cur->dirty);
    }

    // Check that a cache operation hasn't left anything "locked"
    if (task_local) assert(cur->entryReservedByTask != task_local);
    else assert(cur->entryReservedByTask == NULL);

    forward_count++;
  }
  for( cur = tail; cur; cur = cur->prev ) {
    if( cur->prev ) assert(cur->prev->next == cur);
    reverse_count++;
  }

  assert(forward_count == reverse_count);
  return forward_count;
}

static
void validate_pending(struct rdcache_s* cache)
{
  int i = 0;
  int count = fifo_circleb_count(cache->pending_first_entry,
                                 cache->pending_last_entry,
                                 cache->pending_len);
  unsigned int len_mask = cache->pending_len - 1;

  cache_seqn_t last_sn = NO_SEQUENCE_NUMBER;

  // check len_mask is a power of 2
  assert( !(cache->pending_len & len_mask));
  for (i = 0; i < count; i++) {
    unsigned int idx = (cache->pending_first_entry + i) & len_mask;
    cache_seqn_t cur_sn = cache->pending_sequence_numbers[idx];
    if (i != 0) {
      assert(last_sn <= cur_sn);
    }
    last_sn = cur_sn;
  }
}
#endif // if VERIFY

// aka verify_cache cache_verify cache_validate
static
void validate_cache(struct rdcache_s* tree,
                    chpl_cache_taskPrvData_t* task_local)
{
#if VERIFY
  int count;
  int in_ain;
  int in_aout;
  int in_am;
  int num_used_pages = 0;
  int num_dirty = 0;
  int table_slots;
  raddr_t raddr_mask;

  raddr_mask = CACHEPAGE_MASK;
  raddr_mask = ~raddr_mask;

  table_slots = 1 << tree->table_bits;

  // 0: All tree entries must be in either Ain, Aout, or Am,
  //    and num_entries is correct for each top entry.
  for (int i = 0; i < table_slots; i++) {
    struct cache_table_slot_s* slot = &tree->table[i];
    for (int j = 0; j < TABLE_ENTRIES_PER_SLOT; j++) {
      struct cache_table_entry_s* entry = &slot->m[j];
      struct cache_entry_s* bottom_cur = offset_to_entry(tree, entry->offset);

      // check that the address matches the entry
      if (bottom_cur) {
        assert(entry->raddr == bottom_cur->base.raddr);
        assert(entry->node == bottom_cur->base.node);
      }

      // check that the table entry's list bit is set appropriately
      if (bottom_cur && bottom_cur->base.next) {
        assert((entry->offset & 1) == 1);
      } else {
        assert((entry->offset & 1) == 0);
      }

      while(bottom_cur) {
        // check raddr is page-aligned
        assert(bottom_cur->base.raddr == (bottom_cur->base.raddr & raddr_mask));

        // Check that it is in ain, aout, or am.
        in_ain = find_in_queue(tree->ain_head, bottom_cur);
        in_aout = find_in_queue(tree->aout_head, bottom_cur);
        in_am = find_in_queue(tree->am_lru_head, bottom_cur);
        assert( in_ain || in_aout || in_am );
        if( in_ain ) assert( bottom_cur->queue == QUEUE_AIN );
        if( in_aout ) assert( bottom_cur->queue == QUEUE_AOUT );
        if( in_am ) assert( bottom_cur->queue == QUEUE_AM );
        assert( bottom_cur->queue != QUEUE_FREE );
        if( bottom_cur->page ) num_used_pages++;
        if( bottom_cur->dirty ) num_dirty++;
        bottom_cur = (struct cache_entry_s*) bottom_cur->base.next;
        count++;
      }
    }
  }

  // 1: Entries in Ain must be in the tree
  in_ain = validate_queue(tree, task_local,
                          tree->ain_head, tree->ain_tail, QUEUE_AIN);
  assert( in_ain == tree->ain_current );
  // 2: Entries in Aout must be in the tree
  in_aout = validate_queue(tree, task_local,
                           tree->aout_head, tree->aout_tail, QUEUE_AOUT);
  assert( in_aout == tree->aout_current );
  // 3: Entries in Am must be in the tree
  in_am = validate_queue(tree, task_local,
                         tree->am_lru_head, tree->am_lru_tail, QUEUE_AM);
  assert( in_am == tree->am_current );

  // 4: dirty list must be well-formed
  {
    struct dirty_entry_s* cur;
    int forward_count = 0;
    int reverse_count = 0;
    int with_entry_count = 0;
    int without_entry_count = 0;

    for( cur = tree->dirty_lru_head; cur; cur = cur->next ) {
      if( cur->next ) assert(cur->next->prev == cur);
      forward_count++;
      if( cur->entry ) {
        with_entry_count++;
        assert(without_entry_count == 0); // used ones before free ones.
      } else without_entry_count++;
    }
    for( cur = tree->dirty_lru_tail; cur; cur = cur->prev ) {
      if( cur->prev ) assert(cur->prev->next == cur);
      reverse_count++;
    }
    assert(forward_count == reverse_count);
    assert(num_dirty == tree->num_dirty_pages);
    assert(with_entry_count == tree->num_dirty_pages);
  }

  // 5: must not lose entries. Check that the entry free list
  // contains the right number of entries.
  {
    struct cache_list_entry_s* cur;
    int num_free_entries = 0;
    for (cur = tree->free_entries_head; cur; cur = cur->next) {
      num_free_entries++;
    }
    assert( in_ain + in_aout + in_am + num_free_entries == tree->max_entries );
  }

  // 6: must not lose pages
  {
    struct page_list_s* cur;
    int num_free_pages = 0;
    for( cur = tree->free_pages_head; cur; cur = cur->next ) {
      assert(cur->page != NULL);
      num_free_pages++;
    }
    assert( num_used_pages + num_free_pages == tree->max_pages );
  }

  // 7: pending queue must have sequence numbers in order
  // (or else do_wait_for would need updating to check all pending
  //  sequence numbers)
  validate_pending(tree);

#endif
}

static
void do_wait_for(struct rdcache_s* cache, cache_seqn_t sn)
{

  TRACE_NB_PRINT(("%d: task %d wait_for(%i) completed=%i\n",
                  chpl_nodeID, (int)chpl_task_getId(),
                  (int) sn, (int) cache->completed_request_number));

  // Do nothing if there are no pending entries.
  if (cache->pending_first_entry < 0 || cache->pending_last_entry < 0) {
    return;
  }

  // Do nothing if we don't have a valid sequence number to wait for.
  if (sn == NO_SEQUENCE_NUMBER) {
    return;
  }

  // Do nothing if we have already completed sn.
  if (sn <= cache->completed_request_number) {
    return;
  }

#if VERIFY
  validate_pending(cache);
#endif

  // If we have any pending requests with sequence number <= sn,
  // wait for them to complete.
  while (1) {
    int index;
    int last;

    index = cache->pending_first_entry;
    if (index == -1) break;

    // If the first entry's sequence number is earlier than sn
    // and that comm event isn't complete yet, then wait for some
    if (cache->pending_sequence_numbers[index] <= sn &&
        !chpl_comm_test_nb_complete(cache->pending[index])) {
      // Wait for some requests
      last = cache->pending_last_entry;
      if (last < index) last = cache->pending_len - 1;

      // Wait for some requests to complete.
      // (this could cause a different task body to run)
      chpl_comm_wait_nb_some(&cache->pending[index], last - index + 1);

      if (EXTRA_YIELDS) {
        TRACE_YIELD_PRINT(("%d: task %d cache %p yielding in do_wait_for "
                           "for chpl_comm_wait_nb_some\n",
                           chpl_nodeID, (int) chpl_task_getId(), cache));

        chpl_task_yield();

        TRACE_YIELD_PRINT(("%d: task %d cache %p back in do_wait_for\n",
                          chpl_nodeID, (int) chpl_task_getId(), cache));
      }

      // continue the loop
    }

    // Whether we waited above or not, if the first entry's event
    // is already complete, then remove it from the queue.
    if (chpl_comm_test_nb_complete(cache->pending[index])) {
      fifo_circleb_pop(&cache->pending_first_entry,
                       &cache->pending_last_entry,
                       cache->pending_len);
      cache->completed_request_number =
        seqn_max(cache->completed_request_number,
                 cache->pending_sequence_numbers[index]);

      TRACE_NB_PRINT(("%d: task %d wait_for popped %i completed=%i\n",
                      chpl_nodeID, (int)chpl_task_getId(),
                      (int) cache->pending_sequence_numbers[index],
                      (int) cache->completed_request_number));
    }

    // Stop if we have an uncompleted request for a later sequence number
    if (cache->pending_sequence_numbers[index] > sn) {
      DEBUG_PRINT(("wait_for stopped at %i\n", index));
      break;
    }
  }
}


// try to "lock" the entry
//  * if entry->entryReservedByTask is NULL, set it to task_local
//    and return 1 (indicating the lock was taken)
//
//  * otherwise, return 0
//    (and assert that this task doesn't already have the lock).
//    If 0 is returned, the caller should loop calling chpl_task_yield.
//
// This lock protects the entry from being evicted by another task
// in the same pthread.
static
int try_reserve_entry(struct rdcache_s* cache,
                      chpl_cache_taskPrvData_t* task_local,
                      struct cache_entry_s* entry) {

  if (entry->entryReservedByTask != NULL) {
    assert(entry->entryReservedByTask != task_local);
    return 0;
  } else {
    // now entry->entryReservedByTask == NULL
    entry->entryReservedByTask = task_local; // "lock"
    return 1;
  }
}

// "unlock" the entry
static
void unreserve_entry(struct rdcache_s* cache,
                   chpl_cache_taskPrvData_t* task_local,
                   struct cache_entry_s* entry) {
  assert(entry->entryReservedByTask == task_local);
  entry->entryReservedByTask = NULL;
}


// For the region of this page in raddr,len, we complete any pending/not
// started operations that possibly overlap with that region.
// If FLUSH_EVICT or FLUSH_INVALIDATE_PAGE is set, we will ignore the region.
//
// assumes that the entry is already locked
static
void flush_entry(struct rdcache_s* cache,
                 chpl_cache_taskPrvData_t* task_local,
                 struct cache_entry_s* entry, int op,
                 raddr_t raddr, int32_t len_in)
{
  struct page_list_s* free_page_list_entry;
  unsigned char* page;
  uintptr_t start;
  struct dirty_entry_s* dirty;
  uint64_t *dirty_bits;
  raddr_t skip = raddr & CACHEPAGE_MASK;
  uintptr_t len = len_in;
  raddr_t line_start, line_last, line_next;
  uintptr_t num_lines, skip_lines;
  chpl_comm_nb_handle_t handle;
  uintptr_t got_skip, got_len;

  assert(entry->entryReservedByTask == task_local);

  DEBUG_PRINT(("flush_entry(%p, %i, %p, %i)\n",
               entry, op, (void*) raddr, (int) len));
#ifdef DUMP
  printf("starting flush_entry, entry is:\n");
  cache_entry_print(entry, " toflush ", 1);
#endif

  if( op & FLUSH_DO_PAGE ) {
    // Ignore the passed-in raddr/len.
    skip = 0; len = CACHEPAGE_SIZE;
  }

  line_start = round_down_to_mask(skip, CACHELINE_MASK);
  line_last = round_down_to_mask(skip+len-1, CACHELINE_MASK);
  line_next = line_last + CACHELINE_SIZE;
  num_lines = (line_next - line_start) >> CACHELINE_BITS;
  skip_lines = line_start >> CACHELINE_BITS;

  if( op & FLUSH_DO_CLEAR_DIRTY ) {
    // If there are dirty bits that could overlap our region,
    // start writes for all dirty bits

    if( entry->dirty ) {
      dirty = entry->dirty;
      page = entry->page;
      assert(page);

      dirty_bits = dirty->dirty;
      if( len == CACHEPAGE_SIZE ||
          any_set_for_skip_len(dirty_bits, skip, len, CACHEPAGE_BITMASK_WORDS) ) {
        start = 0;
        while( get_skip_len_for_valids(dirty_bits, start, &got_skip, &got_len, CACHEPAGE_BITMASK_WORDS) ) {

          start = got_skip;
          // Start a put for len bytes starting at page + start
          DEBUG_PRINT(("chpl_comm_start_put(%p, %i, %p, %i)\n",
                 page+start, entry->base.node, (void*) (entry->raddr+start),
                 (int) got_len));

          // Note: chpl_comm_put_nb, pending_push can yield
          handle =
            chpl_comm_put_nb(page+start, /*local addr*/
                             entry->base.node,
                             (void*)(entry->base.raddr+start),
                             got_len /*size*/,
                             CHPL_COMM_UNKNOWN_ID, -1, 0);
          if (EXTRA_YIELDS) {
            TRACE_YIELD_PRINT(("%d: task %d cache %p yielding in flush_entry "
                               "for chpl_comm_put_nb\n",
                               chpl_nodeID, (int) chpl_task_getId(), cache));

            chpl_task_yield();

            TRACE_YIELD_PRINT(("%d: task %d cache %p back in flush_entry\n",
                               chpl_nodeID, (int) chpl_task_getId(), cache));
          }

          // Save the handle in the list of pending requests.
          entry->max_put_sequence_number = pending_push(cache, handle);

          // Move past this region of 1s in dirty bits.
          start = got_skip + got_len;
        }
        // Now remove the dirty structure and put it back on its free list.
        // This has the effect of clearing the dirty bits.
        DOUBLE_REMOVE(cache, dirty, dirty_lru);
        dirty->entry = NULL;
        entry->dirty = NULL;
        DOUBLE_PUSH_TAIL(cache, dirty, dirty_lru);
        // ... and decrement the number of dirty pages.
        cache->num_dirty_pages--;
      }
    }
  }

  // If there are pending ops on this page that possibly overlap
  // with the write in question, wait for them now.
  if( op & FLUSH_DO_PENDING ) {
    // If there is a pending put sequence number not completed, we must
    // wait for it now (since we don't know which region it corresponded to).
    wait_for(cache, entry->max_put_sequence_number);

    // If the previously valid cache lines overlap with the
    // request, we must wait for them now.
    if( any_valid_lines(entry->valid_lines, skip_lines, num_lines) ) {
      wait_for(cache, entry->max_prefetch_sequence_number);
    }
  }

  // If invalidating, clear valid bits.
  if( op & FLUSH_DO_INVALIDATE ) {
    if( len == CACHEPAGE_SIZE ) {
      entry->readahead_skip = 0;
      entry->readahead_len = 0;
      entry->min_sequence_number = NO_SEQUENCE_NUMBER;
      entry->max_put_sequence_number = NO_SEQUENCE_NUMBER;
      entry->max_prefetch_sequence_number = NO_SEQUENCE_NUMBER;
      memset(entry->valid_lines, 0, CACHE_LINES_PER_PAGE_BITMASK_WORDS*sizeof(uint64_t));
    } else {
      unset_valid_lines(entry->valid_lines, skip_lines, num_lines);
    }
  }

  // If evicting, remove the page from the cache and put it on a free list.
  if( op & FLUSH_DO_EVICT ) {
    // But, our entry no longer can have a page associated with it.
    page = entry->page;
    entry->page = NULL;
    if (page) {
      free_page_list_entry = cache->free_page_list_entries_head;
      SINGLE_POP_HEAD(cache, free_page_list_entries);
      free_page_list_entry->page = page;
      SINGLE_PUSH_HEAD(cache, free_page_list_entry, free_pages);
    }
  }

#ifdef DUMP
  printf("after flush_entry, entry is:\n");
  cache_entry_print(entry, " flushed ", 1);
#endif
}

// Call this function to tell the cache that we are 'use'ing an entry;
// it will move it to the front of any LRU queue
// (particularly for the Am queue).
static
void use_entry(struct rdcache_s* cache,
               struct cache_entry_s* entry)
{
  // If it's on the Am queue, move it to the front of the Am queue.
  if( entry->queue == QUEUE_AM ) {
    DOUBLE_REMOVE(cache, entry, am_lru);
    DOUBLE_PUSH_HEAD(cache, entry, am_lru);
  }
  // Otherwise (it is in Ain) so leave it where it is.
}

// Plumb a cache entry into the tree.
// If aout_entry is not NULL, we will replace an existing entry from Aout
// if one is present (and aout_entry is probably such an entry, but
// cache manipulations might have removed it, e.g.). If the entry from Aout
// exists, it will be moved to Am.
//
// Otherwise, this will create a new entry in Ain and it will assume
// that the entry does not currently exist in the cache.
//
// Assumes caller has ensured there is a free page and a free entry.
//
// The returned entry is always "reserved" i.e. entryInUse = 1.
static
struct cache_entry_s* make_entry(struct rdcache_s* tree,
                                 c_nodeid_t node, raddr_t raddr,
                                 unsigned char* page,
                                 struct cache_entry_s* aout_entry)
{
  struct cache_table_entry_s* prev_table;
  struct cache_entry_s *bottom_match;
  struct cache_entry_s *bottom_tmp;
  struct cache_entry_s *next;

  assert(raddr != 0);

  bottom_match = NULL;
  if (aout_entry) {
    if (aout_entry->base.raddr == raddr && aout_entry->base.node == node &&
        aout_entry->queue == QUEUE_AOUT) {
      // OK to reuse aout_entry - it was not removed.
      bottom_match = aout_entry;
    }
  }

  if( bottom_match ) {
    // If X is in A1out then find space for X and add it to the head of Am
    assert( bottom_match->base.node == node );
    assert( bottom_match->base.raddr == raddr );
    // We shouldn't be replacing something in Ain or Am; use use_entry instead
    assert(bottom_match->queue == QUEUE_AOUT);

    DEBUG_PRINT(("%d: Found %p in Aout\n", chpl_nodeID, (void*) raddr));
    // add X to the head of Am
    DOUBLE_REMOVE(tree, bottom_match, aout);
    tree->aout_current--;
    DOUBLE_PUSH_HEAD(tree, bottom_match, am_lru);
    tree->am_current++;

    bottom_match->queue = QUEUE_AM;
    bottom_match->entryReservedByTask = NULL;
    bottom_match->readahead_skip = 0;
    bottom_match->readahead_len = 0;
    // Set the page to the one the caller already allocated
    bottom_match->page = page;
    // Clear the valid lines
    memset(&bottom_match->valid_lines, 0, sizeof(uint64_t)*CACHE_LINES_PER_PAGE_BITMASK_WORDS);
    // Clear the dirty pointer and sequence numbers.
    bottom_match->dirty = NULL;
    bottom_match->min_sequence_number = NO_SEQUENCE_NUMBER;
    bottom_match->max_put_sequence_number = NO_SEQUENCE_NUMBER;
    bottom_match->max_prefetch_sequence_number = NO_SEQUENCE_NUMBER;
  } else {
    // Else If X is in no queue then find space for X and add it to A1in
    DEBUG_PRINT(("%d: Found %p nowhere\n", chpl_nodeID, (void*) raddr));

    // This is our new entry...
    bottom_tmp = allocate_entry(tree);
    // We know that this entry does not exist in the tree,
    // so we only need to lookup where to add it.
    prev_table = lookup_entry_adding(tree, node, raddr);

    // Fill in the entry...
    bottom_tmp->base.node = node;
    bottom_tmp->base.raddr = raddr;
    bottom_tmp->base.next = NULL;

    bottom_tmp->queue = QUEUE_AIN;
    bottom_tmp->entryReservedByTask = NULL;
    bottom_tmp->readahead_skip = 0;
    bottom_tmp->readahead_len = 0;

    bottom_tmp->next = NULL;
    bottom_tmp->prev = NULL;
    bottom_tmp->page = page;
    memset(&bottom_tmp->valid_lines, 0, sizeof(uint64_t)*CACHE_LINES_PER_PAGE_BITMASK_WORDS);
    bottom_tmp->dirty = NULL;
    bottom_tmp->min_sequence_number = NO_SEQUENCE_NUMBER;
    bottom_tmp->max_put_sequence_number = NO_SEQUENCE_NUMBER;
    bottom_tmp->max_prefetch_sequence_number = NO_SEQUENCE_NUMBER;

    // Add it to the AIN queue.
    DOUBLE_PUSH_HEAD(tree, bottom_tmp, ain);
    tree->ain_current++;

    // Put it as the first element in the appropriate table entry
    // using the prev_table entry; adjust the next_offset for anything
    // that was there already.

    // Link to next hashtable element, adding not replacing.
    next = offset_to_entry(tree, prev_table->offset);
    bottom_tmp->base.next = &next->base;
    // Replace the lookup table entry
    prev_table->raddr = raddr;
    prev_table->node = node;
    prev_table->offset = entry_to_offset(tree, bottom_tmp);
    if (bottom_tmp->base.next)
      prev_table->offset |= 1; // set list flag

    bottom_match = bottom_tmp;
    DEBUG_PRINT(("  added a new bottom_index_entry: %p\n", bottom_match));
  }

  return bottom_match;
}

typedef enum {
  CACHE_OP_PUT = 1,
  CACHE_OP_GET = 2,
  CACHE_OP_PREFETCH = 3,
  CACHE_OP_MOCK_GET = 4,
} cache_op_t;

// Reserves an entry, creating it if necessary.
//  If one is found, "use" it in the cache algorithm.
//  If none is found, create a new entry and page.
//
//  hint_entry is an entry to check first (before traversing the tree)
//  for a match.
//
//  Returns a reserved entry (with entryInUse=1) that needs to
//  be unreserved at the end of the operation.
static
struct cache_entry_s* get_reserved_entry(struct rdcache_s* cache,
                                         chpl_cache_taskPrvData_t* task_local,
                                         struct cache_entry_s* hint_entry,
                                         c_nodeid_t node, raddr_t ra_page,
                                         cache_op_t op)
{

  assert(task_local != NULL);

  while (1) {
    struct cache_entry_s* entry = NULL;
    struct cache_entry_s* aout_entry = NULL;

    // If there is a hint entry, it is OK to reuse it
    // as long as it has the correct raddr and node and
    // is not in the free queue.
    // This is only the case because all entries are preallocated
    // (so if one is freed or reused elsewhere these values will differ).
    if (hint_entry) {
      if (hint_entry->base.raddr == ra_page && hint_entry->base.node == node &&
          hint_entry->queue != QUEUE_FREE) {
        entry = hint_entry;
      }
    }

    // if we don't have an entry yet, look up node/page in the tree
    if (entry == NULL) {
      entry = lookup_entry(cache, node, ra_page);
      hint_entry = entry;
    }

    // consider Aout entries as a different thing here
    if (entry && !entry->page) {
      aout_entry = entry;
      entry = NULL;
    }

    if (entry) {
      // entry was in Ain or Am

      if (!try_reserve_entry(cache, task_local, entry)) {
        // could not "lock" entry so try again
        TRACE_YIELD_PRINT(("%d: task %d cache %p yielding in get_reserved0\n",
                           chpl_nodeID, (int) chpl_task_getId(), cache));

        chpl_task_yield();

        TRACE_YIELD_PRINT(("%d: task %d cache %p back in get_reserved0\n",
                           chpl_nodeID, (int) chpl_task_getId(), cache));
        continue;
      }

      // "lock"ed entry

      // move entry to the front of the cache if needed
      use_entry(cache, entry);
      return entry;

    } else if (aout_entry) {
      // entry was in Aout

      // make sure we have a free page
      if (cache->free_pages_head == NULL) {
        // free a page and try again to find the entry
        ensure_free_page(cache, task_local, /* give_up_if_locked */ 0);
        // above call may yield, so try everything again
        continue;
      }

      {
        // allocate_page can't yield.
        // above code ensured a free page exists.
        unsigned char* page = allocate_page(cache);
        assert(page);
        entry = make_entry(cache, node, ra_page, page, aout_entry);
        assert(entry->entryReservedByTask == NULL); // new, should be unlocked

        if (!try_reserve_entry(cache, task_local, entry)) {
          assert(0); // unexpected case
        }

        // "lock"ed entry
        return entry;
      }
    } else {
      // no entry exists yet

      // make sure we have a free entry if needed
      if (cache->free_entries_head == NULL) {
        // free an entry and try again to find the entry
        reclaim(cache, task_local, /* give_up_if_locked */ 0);
        // above call may yield, so try everything again
        continue;
      }

      // make sure we have a free page if needed
      if (cache->free_pages_head == NULL) {
        // free a page and try again to find the entry
        ensure_free_page(cache, task_local, /* give_up_if_locked */ 0);
        // above call may yield, so try everything again
        continue;
      }

      {
        // allocate_page and make_entry can't yield.
        // above code ensured a free page & free entry exist.
        unsigned char* page = allocate_page(cache);
        entry = make_entry(cache, node, ra_page, page, aout_entry);
        assert(entry->entryReservedByTask == NULL); // new, should be unlocked

        if (!try_reserve_entry(cache, task_local, entry)) {
          assert(0); // unexpected case
        }

        // "lock"ed entry
        return entry;
      }
    }
  }

  assert(0); // unexpected case
  return NULL;
}


// PUT size bytes starting at addr into node,raddr
// returns 1 if it was a hit, 0 otherwise
static
int cache_put_in_page(struct rdcache_s* cache,
                      chpl_cache_taskPrvData_t* task_local,
                      unsigned char* addr,
                      c_nodeid_t node, raddr_t raddr, size_t size,
                      int32_t commID, int ln, int32_t fn)
{
  struct cache_entry_s* entry;
  raddr_t ra_page;
  int entry_after_acquire;
  cache_seqn_t sn;
  int hit;

  TRACE_PRINT(("%d: task %d in put_in_page %s:%d get %d bytes from "
               "%d:%p to %p\n",
               chpl_nodeID, (int)chpl_task_getId(), chpl_lookupFilename(fn), ln,
               (int)size, node, (void*)raddr, addr));

  DEBUG_PRINT(("cache_put_in_page %i:%p from %p len %i\n",
               (int) node, (void*) raddr, addr, (int) size));

  assert(chpl_nodeID != node); // should be handled in chpl_gen_comm_put
  assert(size != 0); // should be handled at call site

  // ra_page = raddr of start of the page
  ra_page = round_down_to_mask(raddr, CACHEPAGE_MASK);

  // We will put from raddr to raddr+size

  // get a "lock"ed entry for raddr.
  // the "lock" prevents tasks yielded to from updating it.
  entry = get_reserved_entry(cache, task_local, NULL, node, ra_page,
                             CACHE_OP_PUT);
  assert(entry && entry->page && entry->entryReservedByTask);

  // Is this cache line available for use, based on when we
  // last ran an acquire fence?
  entry_after_acquire = (entry->min_sequence_number >= task_local->last_acquire);

  // If the cache line contains any overlapping writes or prefetches,
  // we must wait for them to complete before we store new data.
  // Nonblocking GETs and PUTs must not have their buffers changed
  // during operation. And, we don't want an earlier prefetch to overwrite
  // our later write!
  // Note, this flush call could yield, but that is OK because
  // entry is reserved.
  flush_entry(cache, task_local, entry,
              entry_after_acquire?FLUSH_PREPARE_PUT:FLUSH_INVALIDATE_PAGE,
              raddr, size);
  // note: flush_entry can yield, but entry is locked
  // no chance of deadlock because above flush might create/wait for comms
  // but does not try to lock another page.
  assert(entry->page && entry->entryReservedByTask == task_local);
  assert(entry->base.raddr == ra_page && entry->base.node == node);

  // Make sure we have a dirty structure.
  if (!entry->dirty) {
    allocate_dirty(cache, task_local, entry);
    // note: allocate_dirty can yield, but entry is locked
    // and won't deadlock because it doesn't wait on an entry
    // (it might start comm events and clear out dirty bits)
    assert(entry->page && entry->entryReservedByTask == task_local);
    assert(entry->base.raddr == ra_page && entry->base.node == node);
    assert(entry->dirty);
    hit = 0;
  } else {
    // Make it the most recently use dirty page.
    use_dirty(cache, entry->dirty);
    hit = 1;
  }

  // Now, set the dirty bits.
  set_valids_for_skip_len(entry->dirty->dirty,
                          raddr & CACHEPAGE_MASK, size,
                          CACHEPAGE_BITMASK_WORDS);

  // Op will be started for this in flush_entry for a dirty page.

  // This will increment next request number so cache events are recorded.
  sn = cache->next_request_number;
  cache->next_request_number++;
  // Set the minimum sequence number so an acquire fence before
  // the next read will cause this write to be disregarded.
  entry->min_sequence_number = seqn_min(entry->min_sequence_number, sn);

  // Copy the data into page.
  chpl_memcpy(entry->page + (raddr-ra_page), addr, size);

  unreserve_entry(cache, task_local, entry);
  entry = NULL; // the below calls can yield & invalidate entry

  // Make sure that there is a dirty page available for next time.
  ensure_free_dirty(cache, task_local);

  // Make sure that there is an available page for next time.
  ensure_free_page(cache, task_local, /* give_up_if_locked */ 0);

  return hit;
}


// returns 1 if all of the page operations were "hit"s
static
int cache_put(struct rdcache_s* cache,
              chpl_cache_taskPrvData_t* task_local,
              unsigned char* addr,
              c_nodeid_t node, raddr_t raddr, size_t size,
              int32_t commID, int ln, int32_t fn)
{
  raddr_t ra_first_page;
  raddr_t ra_last_page;
  raddr_t ra_next_page;
  raddr_t ra_page_end;
  raddr_t ra_page;
  raddr_t requested_start, requested_end, requested_size;
  int hit;
  int all_hits = 1;

  DEBUG_PRINT(("cache_put %i:%p from %p len %i\n",
               (int) node, (void*) raddr, addr, (int) size));

  assert(chpl_nodeID != node); // should be handled in chpl_gen_comm_put

  // And don't do anything if it's a zero-length
  if( size == 0 ) {
    return 1;
  }

  // first_page = raddr of start of first needed page
  ra_first_page = round_down_to_mask(raddr, CACHEPAGE_MASK);
  // last_page = raddr of start of last needed page
  ra_last_page = round_down_to_mask(raddr+size-1, CACHEPAGE_MASK);
  ra_next_page = ra_last_page + CACHEPAGE_SIZE;

  // Try to find it in the cache. Go through one page at a time.
  for( ra_page = ra_first_page;
       ra_page <= ra_last_page;
       ra_page += CACHEPAGE_SIZE ) {

    // We will put from ra_page to ra_page_end
    ra_page_end = (ra_page==ra_last_page)?(ra_next_page):(ra_page+CACHEPAGE_SIZE);

    // Compute the portion of the page that was requested
    requested_start = raddr_max(raddr, ra_page);
    requested_end = raddr_min(raddr+size, ra_page_end);
    requested_size = requested_end - requested_start;

    hit = cache_put_in_page(cache, task_local,
                            (addr==NULL)?(NULL):(addr+(requested_start-raddr)),
                            node, requested_start, requested_size,
                            commID, ln, fn);

    all_hits = all_hits && hit;
  }

  if( VERIFY ) validate_cache(cache, task_local);

#ifdef DUMP
  printf("After cache_put, cache is:\n");
  rdcache_print(cache);
#endif

  return all_hits;
}

static inline
int is_congested(struct rdcache_s* cache)
{
  int have = fifo_circleb_count(cache->pending_first_entry,
                                cache->pending_last_entry,
                                cache->pending_len);
  return have > 3 * cache->pending_len / 2;
}

static
int cache_get(struct rdcache_s* cache,
              chpl_cache_taskPrvData_t* task_local,
              unsigned char * addr,
              c_nodeid_t node, raddr_t raddr, size_t size,
              int sequential_readahead_length,
              int32_t commID, int ln, int32_t fn);

static
void cache_get_trigger_readahead(struct rdcache_s* cache,
                                 chpl_cache_taskPrvData_t* task_local,
                                 c_nodeid_t node,
                                 raddr_t page_raddr,
                                 raddr_t request_raddr, size_t request_size,
                                 // skip < 0 -> reverse, >0 -> forward
                                 readahead_distance_t skip,
                                 readahead_distance_t len,
                                 int32_t commID, int ln, int32_t fn)
{
  // Handle prefetching.
  // See http://www.ece.eng.wayne.edu/~sjiang/Tsinghua-2010/linux-readahead.pdf
  // for inspiration.
  // Here we decide whether or not we are going to prefetch the next page
  // in the forward or backward direction, and we also increase the request
  // size if access looks sequential.

  int next_ra_length;
  int ok;
  raddr_t prefetch_start, prefetch_end;
  size_t page_size = 0;
  uintptr_t request_page, request_len_page, prefetch_page, prefetch_len_page;
  raddr_t miss_addr;

  if( len == 0 ) return;

  INFO_PRINT(("%i trigger readahead(%i, %p, %p, %i, %i, %i, %i)\n",
              (int) chpl_nodeID, (int) node,
              (void*) page_raddr, (void*) request_raddr,
              (int) request_size,
              (int) skip, (int) len, (int) last_acquire));

  // If we are accessing a page that has a readahead condition,
  // trigger that readahead.
  if( ENABLE_READAHEAD && skip && ! is_congested(cache) ) {
    next_ra_length = 2 * len;

    if( next_ra_length > MAX_SEQUENTIAL_READAHEAD_BYTES )
      next_ra_length = MAX_SEQUENTIAL_READAHEAD_BYTES;

    if( skip < 0 )
      next_ra_length = - next_ra_length;

    prefetch_start = page_raddr + skip;
    prefetch_end = prefetch_start + len;

    ok = 0;
    // Assuming we have a request for raddr..raddr+len-1,
    // can we prefetch prefetch_addr..prefetch_addr+prefetch_len-1 ?
    // Checks to see if guard pages are enabled (which makes guarded paged
    // un-gettable) if the prefetch would move out of registered memory
    //  (if chpl_comm_get_segment returns segment information)
    //  or to a new page (if segment information is not available)

    // Can we prefetch len bytes starting at prefetch_raddr?
    // If not, compute the smaller amount that we can prefetch.
    if( !chpl_task_guardPagesInUse() && chpl_comm_addr_gettable(node, (void*)prefetch_start, len) ) {
      ok = 1;
    }

    if( ! ok && request_raddr != 0 && request_size != 0 ) {
      // Adjust prefetch_start/prefetch_end so that it lies on the same
      // system page as the requested data.

      page_size = sys_page_size();

      request_page = round_down_to_mask(request_raddr, page_size-1);
      request_len_page = round_down_to_mask(request_raddr+request_size-1, page_size-1);
      prefetch_page = round_down_to_mask(prefetch_start, page_size-1);
      prefetch_len_page = round_down_to_mask(prefetch_end-1, page_size-1);

      //printf("B raddrp %p raddrlenp %p prep %p prelenp %p\n", (void*)request_page, (void*)request_len_page, (void*)prefetch_page, (void*)prefetch_len_page);
      if( request_page <= prefetch_page &&
          prefetch_len_page <= request_len_page ) {
        ok = 1;
      } else {
        if( prefetch_page < request_page && len < 0 ) {
          prefetch_start = request_page;
          ok = 1;
        }
        if( prefetch_len_page > request_len_page && len > 0 ) {
          prefetch_end = request_len_page+page_size;
          ok = 1;
        }
      }
    }

    //printf("C ok %i prefetch_start %p prefetch_end %p\n",
    //       ok, (void*) prefetch_start, (void*) prefetch_end);

    if( ok && prefetch_start < prefetch_end ) {
      TRACE_READAHEAD_PRINT(("%d: task %d starting readahead from %p to %p\n",
                             chpl_nodeID, (int)chpl_task_getId(),
                             (void*) (prefetch_start), (void*) (prefetch_end)));

      cache_get(cache, task_local,
                /* addr */ NULL /* means prefetch */,
                node, prefetch_start, prefetch_end - prefetch_start,
                next_ra_length,
                commID, ln, fn);
    } else {
      // We could not prefetch, so record a cache miss so
      //  that sequential prefetch will continue when we access
      //  that region. We do this if the last cache miss was
      //  within a system page of where we would like to prefetch.
      //  A possibly better approach here would be to add an entry to
      //  the tree structure indicating that when that entry is loaded,
      //  it should be prefetched...

      if( len < 0 )
        miss_addr = prefetch_start + CACHELINE_SIZE;
      else
        miss_addr = prefetch_end - CACHELINE_SIZE;

      if( cache->last_cache_miss_read_node == node &&
          cache->last_cache_miss_read_addr <= prefetch_start + page_size &&
          cache->last_cache_miss_read_addr + page_size >= prefetch_start ) {
        INFO_PRINT(("%i readahead saving miss read: %i:%p\n",
               (int) chpl_nodeID, node, (void*) miss_addr));
        cache->last_cache_miss_read_addr = miss_addr;
      }
      INFO_PRINT(("%i readahead stops: %i: %p %p\n",
                  (int) chpl_nodeID, node, (void*) prefetch_start, (void*) prefetch_end));
    }
  }
}

static
int should_readahead_extend(uint64_t* valid,
                            uintptr_t skip, uintptr_t len )
{
  if(count_valid_before(valid, skip,
        CACHE_LINES_PER_PAGE_BITMASK_WORDS) > 2)
    return 1;
  if(count_valid_at_after(valid, skip+len,
        CACHE_LINES_PER_PAGE_BITMASK_WORDS) > 2)
    return -1;
  return 0;
}

static
void cache_get_compute_readahead_extend(struct rdcache_s* cache,
                                        chpl_cache_taskPrvData_t* task_local,
                                        struct cache_entry_s* entry,
                                        c_nodeid_t node, raddr_t ra_page,
                                        int sequential_readahead_length,
                                        raddr_t *readahead_skip_inout,
                                        raddr_t *readahead_len_inout,
                                        raddr_t *ra_line_inout,
                                        raddr_t *ra_line_end_inout)
{
  int ra = 0;
  raddr_t readahead_skip = *readahead_skip_inout;
  raddr_t readahead_len = *readahead_len_inout;
  raddr_t ra_line = *ra_line_inout;
  raddr_t ra_line_end = *ra_line_end_inout;

  if( ENABLE_READAHEAD_TRIGGER_WITHIN_PAGE && entry ) {
    ra = should_readahead_extend(entry->valid_lines,
                                 (ra_line - ra_page) >> CACHELINE_BITS,
                                 (ra_line_end - ra_line) >> CACHELINE_BITS);
    if( ra ) {
      INFO_PRINT(("%i readahead trigger within-page direction=%i from %p to %p last %p\n",
           (int) chpl_nodeID, ra, (void*) ra_line, (void*) ra_line_end, (void*) cache->last_cache_miss_read_addr));
    }
  }

  // Note that ENABLE_READAHEAD_TRIGGER_SEQUENTIAL is off by default
  if( ENABLE_READAHEAD_TRIGGER_SEQUENTIAL && ra == 0 &&
      cache->last_cache_miss_read_node == node ) {
    if(cache->last_cache_miss_read_addr < ra_line &&
       ra_line <= cache->last_cache_miss_read_addr + CACHEPAGE_SIZE) {
      // forward prefetch.
      ra = 1;
      INFO_PRINT(("%i readahead trigger forward from %p to %p last %p\n",
             (int) chpl_nodeID, (void*) ra_line, (void*) ra_line_end, (void*) cache->last_cache_miss_read_addr));
    } else if(cache->last_cache_miss_read_addr > ra_line &&
              ra_line >= cache->last_cache_miss_read_addr - CACHEPAGE_SIZE) {
      // reverse prefetch
      ra = -1;
      INFO_PRINT(("readahead trigger reverse from %p to %p last %p\n",
             (void*) ra_line, (void*) ra_line_end, (void*) cache->last_cache_miss_read_addr));
    }
  }

  if( ra == 1 ) {
    // Extend ra_line_end to the end of the current page.
    ra_line_end = ra_page + CACHEPAGE_SIZE;

    readahead_skip = CACHEPAGE_SIZE;
    readahead_len = CACHEPAGE_SIZE;
  } else if(ra == -1) {
    // reverse prefetch
    // Extend ra_line to the start of the current page.
    ra_line = ra_page;

    readahead_skip = -CACHEPAGE_SIZE;
    readahead_len = CACHEPAGE_SIZE;
  }

  *readahead_skip_inout = readahead_skip;
  *readahead_len_inout = readahead_len;
  *ra_line_inout = ra_line;
  *ra_line_end_inout = ra_line_end;
}

// If addr == NULL, this will prefetch.
// This call handles only accesses within a page.
// returns 1 if the request was a "hit"
static
int cache_get_in_page(struct rdcache_s* cache,
                      chpl_cache_taskPrvData_t* task_local,
                      unsigned char * addr,
                      c_nodeid_t node, raddr_t raddr, size_t size,
                      raddr_t ra_first_page, raddr_t ra_last_page,
                      int sequential_readahead_length,
                      int32_t commID, int ln, int32_t fn)
{
  struct cache_entry_s* entry;
  raddr_t ra_page;
  raddr_t ra_line;
  raddr_t ra_line_end;
  int has_data;
  cache_seqn_t sn = NO_SEQUENCE_NUMBER;
  int isprefetch;
  int entry_after_acquire;
  chpl_comm_nb_handle_t handle;
  uintptr_t readahead_len, readahead_skip;

  isprefetch = (addr == NULL);

  TRACE_PRINT(("%d: task %d in get_in_page %s:%d get %d bytes from "
               "%d:%p to %p\n",
               chpl_nodeID, (int)chpl_task_getId(), chpl_lookupFilename(fn), ln,
               (int)size, node, (void*)raddr, addr));

  INFO_PRINT(("%i cache_get_in_page addr %p from %i:%p len %i ra_len %i\n",
               (int) chpl_nodeID, addr, (int) node, (void*) raddr, (int) size, sequential_readahead_length));

  assert(chpl_nodeID != node); // should be handled in chpl_gen_comm_prefetch.
  assert(size != 0); // should be handled at call site

  // compute some information about the remote address

  // ra_page = raddr of start of the page
  ra_page = round_down_to_mask(raddr, CACHEPAGE_MASK);
  // ra_line = raddr of start of first needed line
  ra_line = round_down_to_mask(raddr, CACHELINE_MASK);
  // ra_line_end = ra_last_line + CACHELINE_SIZE
  // ra_last_line = raddr of start of last needed line
  ra_line_end = round_down_to_mask(raddr+size-1, CACHELINE_MASK) +
                CACHELINE_SIZE;

  // Try to find this page data in the cache.
  // We will need from ra_line to ra_line_end.

  // Find the entry in the cache and "lock" it to keep this
  // code simpler. (It would be possible to process some cache-hit GETs
  // without "lock"ing but that would require care about the order in
  // which entry fields are set in other regions of the code that
  // are protected by the "lock").

  // The "lock" prevents another operation that occurs within another
  // task we get to through yielding from doing something with the entry.
  entry = get_reserved_entry(cache, task_local, NULL, node, ra_page,
                             isprefetch?CACHE_OP_PREFETCH:CACHE_OP_GET);
  assert(entry && entry->page && entry->entryReservedByTask);
  assert(entry->base.raddr == ra_page && entry->base.node == node);

  // Is this cache line available for use, based on when we
  // last ran an acquire fence?
  entry_after_acquire = (entry->min_sequence_number >= task_local->last_acquire);

  // Is the relevant data available in the cache line?
  has_data = check_valid_lines(entry->valid_lines,
                               (ra_line - ra_page) >> CACHELINE_BITS,
                               (ra_line_end - ra_line) >> CACHELINE_BITS);

  // If we are doing a GET that is adjacent to the last GET,
  // fetch the rest of the data in this page.
  // Adjust readahead_skip, readahead_len, ra_line, and ra_line_end.
  readahead_skip = 0;
  readahead_len = 0;
  if (ENABLE_READAHEAD &&
      entry_after_acquire &&
      sequential_readahead_length == 0 &&
      !has_data &&
      !(entry && entry->readahead_len)) {

    cache_get_compute_readahead_extend(cache, task_local, entry,
                                       node, ra_page,
                                       sequential_readahead_length,
                                       &readahead_skip, &readahead_len,
                                       &ra_line, &ra_line_end);
  }

  // Check for data in the dirty region
  if( !has_data && entry->dirty ) {
    has_data = all_set_for_skip_len(entry->dirty->dirty,
                                    raddr & CACHEPAGE_MASK,
                                    size,
                                    CACHEPAGE_BITMASK_WORDS);
  }


  if (entry_after_acquire && has_data) {
    // Data is already in cache...  but to do a 'get' for previously
    // prefetched data, we might have to wait for it.
    if (!isprefetch) {
      if (entry->max_prefetch_sequence_number >
          cache->completed_request_number) {
        wait_for(cache, entry->max_prefetch_sequence_number);
        // note: wait_for can yield, but entry is locked
        // no chance of deadlock since wait_for just processes comm events
        assert(entry->page && entry->entryReservedByTask == task_local);
        assert(entry->base.raddr == ra_page && entry->base.node == node);
      }

      // Copy the data out.
      chpl_memcpy(addr, entry->page + (raddr-ra_page), size);

#ifdef DUMP
      {
        // printing out gotten data for debug
        int i = 0;
        unsigned char* ptr = entry->page;
        ptr += (raddr-ra_page);

        printf("%d: task %d cache %p got from %d:%p to %p ",
               chpl_nodeID, (int) chpl_task_getId(), cache,
               node, (void*) ptr, addr);

        for (i = 0; i < size; i++ ) {
          printf("%02x", ptr[i]);
        }
        printf("\n");
      }
#endif

      // If we are accessing a page that has a readahead condition,
      // trigger that readahead.
      if( ENABLE_READAHEAD && entry->readahead_len ) {
        // Remove the readahead trigger from this page since
        // we're starting the readahead now.
        readahead_skip = entry->readahead_skip;
        readahead_len = entry->readahead_len;
        entry->readahead_skip = 0;
        entry->readahead_len = 0;

        // "unlock" the entry
        unreserve_entry(cache, task_local, entry);
        entry = NULL;

        // note - triggering readahead can yield
        cache_get_trigger_readahead(cache, task_local,
                                    node, ra_page,
                                    raddr, size,
                                    readahead_skip, readahead_len,
                                    commID, ln, fn);
        return 1;
      }
    }

    // "unlock" the entry
    unreserve_entry(cache, task_local, entry);
    entry = NULL;
    return 1;
  }

  // If we get here, the data was not available, so
  // Get ready to start a GET !

  // If there was an intervening acquire fence preventing
  // us from using this cache line, we need to mark everything
  // as invalid and clear the min and max request numbers.
  // We also need to wait for pending puts using that data...

  // If the cache line contains any overlapping writes or prefetches,
  // we must wait for them to complete before we request new data.
  // Prefetches might not yet have filled in the data according
  // to the promised valid bits. GETs and PUTs must not have
  // their buffers changed during operation.
  flush_entry(cache, task_local, entry,
              entry_after_acquire?FLUSH_PREPARE_GET:FLUSH_INVALIDATE_PAGE,
              ra_line, ra_line_end-ra_line);
  // note: flush_entry can yield, but entry is locked
  // no chance of deadlock because above flush might create/wait for comms
  // but does not try to lock another page.
  assert(entry->page && entry->entryReservedByTask == task_local);
  assert(entry->base.raddr == ra_page && entry->base.node == node);

  // Now we need to start a get into page.
  // We'll get within ra_page from ra_line to ra_line_end.
  INFO_PRINT(("%i chpl_comm_start_get(%p, %i, %p, %i)\n",
               (int) chpl_nodeID, page+(ra_line-ra_page), node, (void*) ra_line,
               (int) (ra_line_end - ra_line)));

  // Note: chpl_comm_get_nb could cause a different task body to run.
  // That should be OK because we marked entry as "reserved".
  handle = chpl_comm_get_nb(entry->page + (ra_line-ra_page), /*local addr*/
                            node, (void*) ra_line,
                            ra_line_end - ra_line /*size*/,
                            commID, ln, fn);
  if (EXTRA_YIELDS) {
    TRACE_YIELD_PRINT(("%d: task %d cache %p yielding in cache_get_in_page "
                       "for chpl_comm_get_nb\n",
                       chpl_nodeID, (int) chpl_task_getId(), cache));

    chpl_task_yield();

    TRACE_YIELD_PRINT(("%d: task %d cache %p back in cache_get_in_page\n",
                       chpl_nodeID, (int) chpl_task_getId(), cache));
  }
  // note: chpl_comm_get_nb can yield, but entry is locked
  assert(entry->page && entry->entryReservedByTask == task_local);
  assert(entry->base.raddr == ra_page && entry->base.node == node);

  // Set the valid lines
  set_valid_lines(entry->valid_lines,
                  (ra_line - ra_page) >> CACHELINE_BITS,
                  (ra_line_end - ra_line) >> CACHELINE_BITS);

  if (!isprefetch) {
    // This will increment next request number so cache events are recorded.
    sn = cache->next_request_number;
    cache->next_request_number++;
  } else {
    // For a prefetch, store sequence number and record operation handle.

    // This will increment next request number so cache events are recorded.
    sn = pending_push(cache, handle);
    // note: pending_push can yield, but entry is locked
    assert(entry->page && entry->entryReservedByTask == task_local);
    assert(entry->base.raddr == ra_page && entry->base.node == node);

    entry->max_prefetch_sequence_number = seqn_max(entry->max_prefetch_sequence_number, sn);
  }

  // Set the minimum sequence number
  entry->min_sequence_number = seqn_min(entry->min_sequence_number, sn);

  // Decide what to store in the readahead trigger for this page
  // if we are currently doing a readahead.
  if( ENABLE_READAHEAD ) {
    if( sequential_readahead_length > 0 && ra_page == ra_first_page ) {
      // forward readahead
      readahead_skip = size;
      readahead_len = sequential_readahead_length;
    }
    if( sequential_readahead_length < 0 && ra_page == ra_last_page ) {
      // reverse readahead
      readahead_skip = sequential_readahead_length -
                       (ra_last_page - ra_first_page);
      readahead_len = -sequential_readahead_length; // positive.
    }
  }

  // Save readahead trigger for this page if we have one to set.
  // It could come from just above or from readahead start.
  if( ENABLE_READAHEAD && readahead_len ) {

    INFO_PRINT(("%i saving for %i:%p skip %i len %i\n",
                 (int) chpl_nodeID, (int) node, (void*) entry->raddr,
                 (int) readahead_skip, (int) readahead_len));

    entry->readahead_skip = readahead_skip;
    entry->readahead_len = readahead_len;
  }

  // Update the last read location on a miss
  // (as long as there was not an intervening acquire)
  if( entry_after_acquire && sequential_readahead_length == 0 ) {
    cache->last_cache_miss_read_node = node;
    cache->last_cache_miss_read_addr = ra_line;
  }

  // Make sure that there is an available page for next time,
  // but do it without evicting entry (that we are working with).
  // This could happen if entry is the last element of Ain.
  //
  // To prevent deadlock, this call does nothing if the page we would
  // reclaim is locked. That is OK because this call is here for performance,
  // and not correctness, and so be skipped in that event.
  // It also causes 'entry' not to be reclaimed if it is
  // the last thing in Ain.
  ensure_free_page(cache, task_local, /* give_up_if_locked */ 1);
  // note: ensure_free_page can yield, but entry is locked
  assert(entry->page && entry->entryReservedByTask == task_local);
  assert(entry->base.raddr == ra_page && entry->base.node == node);

  if (!isprefetch) {
    // If we're not prefetching... wait for the get to complete and copy it
    // back out of the cache.

    chpl_comm_wait_nb_some(&handle, 1);
    if (EXTRA_YIELDS) {
      TRACE_YIELD_PRINT(("%d: task %d cache %p yielding in cache_get_in_page "
                         "for chpl_comm_wait_nb_some\n",
                         chpl_nodeID, (int) chpl_task_getId(), cache));

      chpl_task_yield();

      TRACE_YIELD_PRINT(("%d: task %d cache %p back in cache_get_in_page\n",
                         chpl_nodeID, (int) chpl_task_getId(), cache));
    }

    // note: chpl_comm_wait_nb_some can yield, but entry is locked
    assert(entry->page && entry->entryReservedByTask == task_local);
    assert(entry->base.raddr == ra_page && entry->base.node == node);

    // Then, copy it out.
    chpl_memcpy(addr, entry->page + (raddr-ra_page), size);

#ifdef DUMP
    {
      // printing out gotten data for debug
      int i = 0;
      unsigned char* ptr = entry->page;
      ptr += (raddr-ra_page);

      printf("%d: task %d cache %p got from %d:%p to %p ",
             chpl_nodeID, (int) chpl_task_getId(), cache,
             node, (void*) ptr, addr);

      for (i = 0; i < size; i++ ) {
        printf("%02x", ptr[i]);
      }
      printf("\n");
    }
#endif

  }

  unreserve_entry(cache, task_local, entry);
  entry = NULL;

  // TODO: is this call necessary?
  ensure_free_page(cache, task_local, /* give_up_if_locked */ 0);

  return 0;
}


// If addr == NULL, this will prefetch.
// Returns 1 if all of the data was in the cache.
static
int cache_get(struct rdcache_s* cache,
              chpl_cache_taskPrvData_t* task_local,
              unsigned char * addr,
              c_nodeid_t node, raddr_t raddr, size_t size,
              int sequential_readahead_length,
              int32_t commID, int ln, int32_t fn)
{
  raddr_t ra_first_page;
  raddr_t ra_last_page;
  raddr_t ra_page;
  raddr_t requested_start, requested_end, requested_size;
  raddr_t ra_last_line;
  raddr_t ra_next_line;
  raddr_t ra_line_end;
  int isprefetch = (addr == NULL);
#ifdef TIME
  struct timespec start_get1, start_get2, wait1, wait2;
#endif
  int hit;
  int all_hits = 1;

  INFO_PRINT(("%i cache_get addr %p from %i:%p len %i ra_len %i\n",
               (int) chpl_nodeID, addr, (int) node, (void*) raddr, (int) size, sequential_readahead_length));

  assert(chpl_nodeID != node); // should be handled in chpl_gen_comm_prefetch.

  // And don't do anything if it's a zero-length
  if( size == 0 ) {
    return 1;
  }

  // first_page = raddr of start of first needed page
  ra_first_page = round_down_to_mask(raddr, CACHEPAGE_MASK);
  // last_page = raddr of start of last needed page
  ra_last_page = round_down_to_mask(raddr+size-1, CACHEPAGE_MASK);
  // last_line = raddr of start of last needed line
  ra_last_line = round_down_to_mask(raddr+size-1, CACHELINE_MASK);
  ra_next_line = ra_last_line + CACHELINE_SIZE;

  // If the request is too large to reasonably fit in the cache, limit
  // the amount of data prefetched. (or do nothing?)
  if( isprefetch && (ra_last_page-ra_first_page)/CACHEPAGE_SIZE+1 > MAX_PAGES_PER_PREFETCH ) {
    ra_last_page = ra_first_page + CACHEPAGE_SIZE*MAX_PAGES_PER_PREFETCH;
  }

  // Try to find it in the cache. Go through one page at a time.
  for( ra_page = ra_first_page;
       ra_page <= ra_last_page;
       ra_page += CACHEPAGE_SIZE ) {


    // We will need from ra_line to ra_line_end.
    ra_line_end = (ra_page==ra_last_page)?(ra_next_line):(ra_page+CACHEPAGE_SIZE);
    // Compute the portion of the page that was requested
    requested_start = raddr_max(raddr,ra_page);
    requested_end = raddr_min(raddr+size,ra_line_end);
    requested_size = requested_end - requested_start;

    hit = cache_get_in_page(cache, task_local,
                            (addr==NULL)?(NULL):(addr+(requested_start-raddr)),
                            node, requested_start, requested_size,
                            ra_first_page, ra_last_page,
                            sequential_readahead_length,
                            commID, ln, fn);

    all_hits = all_hits && hit;
  }

  if( VERIFY ) validate_cache(cache, task_local);

#ifdef DUMP
  printf("After cache_get cache is:\n");
  rdcache_print(cache);
#endif

  return all_hits;
}

// This is intended to match cache_get but
//  * it will never prefetch
//  * it doesn't actually GET; just memsets to 0 instead.
// Returns 1 if the data was in the cache.
static
int mock_get(struct rdcache_s* cache,
             chpl_cache_taskPrvData_t* task_local,
             c_nodeid_t node, raddr_t raddr, size_t size,
             cache_seqn_t last_acquire,
             int sequential_readahead_length,
             int32_t commID, int ln, int32_t fn)
{
  struct cache_entry_s* entry;
  struct cache_entry_s* aout_entry;
  raddr_t ra_first_page;
  raddr_t ra_last_page;
  raddr_t ra_page;
  raddr_t requested_start, requested_end, requested_size;
  raddr_t ra_first_line;
  raddr_t ra_last_line;
  raddr_t ra_next_line;
  raddr_t ra_line;
  raddr_t ra_line_end;
  int has_data;
  unsigned char* page;
  cache_seqn_t sn = NO_SEQUENCE_NUMBER;
  int entry_after_acquire;
#ifdef TIME
  struct timespec start_get1, start_get2, wait1, wait2;
#endif
  int all_in_cache;

  INFO_PRINT(("%i mock_get addr %p from %i:%p len %i ra_len %i\n",
               (int) chpl_nodeID, addr, (int) node, (void*) raddr, (int) size, sequential_readahead_length));

  if (chpl_nodeID == node)
    return 1;

  // And don't do anything if it's a zero-length
  if( size == 0 ) {
    return 1;
  }

  all_in_cache = 1;

  // first_page = raddr of start of first needed page
  ra_first_page = round_down_to_mask(raddr, CACHEPAGE_MASK);
  // last_page = raddr of start of last needed page
  ra_last_page = round_down_to_mask(raddr+size-1, CACHEPAGE_MASK);
  // first_line = raddr of start of first needed line
  ra_first_line = round_down_to_mask(raddr, CACHELINE_MASK);
  // last_line = raddr of start of last needed line
  ra_last_line = round_down_to_mask(raddr+size-1, CACHELINE_MASK);
  ra_next_line = ra_last_line + CACHELINE_SIZE;

  // Try to find it in the cache. Go through one page at a time.
  for( ra_page = ra_first_page, ra_line = ra_first_line;
       ra_page <= ra_last_page;
       ra_page += CACHEPAGE_SIZE, ra_line = ra_page ) {

    // We will need from ra_line to ra_line_end.
    ra_line_end = (ra_page==ra_last_page)?(ra_next_line):(ra_page+CACHEPAGE_SIZE);
    // Compute the portion of the page that was requested
    requested_start = raddr_max(raddr,ra_page);
    requested_end = raddr_min(raddr+size,ra_line_end);
    requested_size = requested_end - requested_start;

    // Is the page in the tree?
    entry = lookup_entry(cache, node, ra_page);
    aout_entry = NULL;
    page = NULL;

    // Ignore entries in Aout for now.
    if( entry && ! entry->page ) {
      aout_entry = entry;
      entry = NULL;
    }

    if( entry ) {
      // Is this cache line available for use, based on when we
      // last ran an acquire fence?
      entry_after_acquire = ( entry->min_sequence_number >= last_acquire );

      // Is the relevant data available in the cache line?
      has_data = check_valid_lines(entry->valid_lines,
                                   (ra_line - ra_page) >> CACHELINE_BITS,
                                   (ra_line_end - ra_line) >> CACHELINE_BITS);
    } else {
      entry_after_acquire = 1;
      has_data = 0;
    }

    //printf("%i entry is %p after_acquire %i has_data %i\n", chpl_nodeID, entry, entry_after_acquire, has_data);

    if( entry ) {
      page = entry->page;

      // Check for data in the dirty region
      if( !has_data && entry->dirty ) {
        has_data = all_set_for_skip_len(entry->dirty->dirty,
                                        requested_start & CACHEPAGE_MASK,
                                        requested_size,
                                        CACHEPAGE_BITMASK_WORDS);
      }

      if( entry_after_acquire && has_data ) {
        // Data is already in cache...  but to do a 'get' for previously
        // prefetched data, we might have to wait for it.
        {
          if( entry->max_prefetch_sequence_number > cache->completed_request_number ) {
            wait_for(cache, entry->max_prefetch_sequence_number);
          }
        }
        // If the cache line is in Am, move it to the front of Am.
        use_entry(cache, entry);
        // Would copy the data out here, but not in mock get
        continue; // Move on to the next page.
      }

      // Get ready to start a get !

      // If there was an intervening acquire fence preventing
      // us from using this cache line, we need to mark everything
      // as invalid and clear the min and max request numbers.
      // We also need to wait for pending puts using that data...

      // If the cache line contains any overlapping writes or prefetches,
      // we must wait for them to complete before we request new data.
      // Prefetches might not yet have filled in the data according
      // to the promised valid bits. GETs and PUTs must not have
      // their buffers changed during operation.
      flush_entry(cache, task_local, entry,
                  entry_after_acquire?FLUSH_PREPARE_GET:FLUSH_INVALIDATE_PAGE,
                  ra_line, ra_line_end-ra_line);
    }

    // Otherwise -- pretend to start a "get" !
    all_in_cache = 0;

    if( ! page ) {
      // get a page from the free list.
      page = allocate_page(cache);
    }

    // Now we need to start a get into page.
    // If we don't have entry set, we will also need to plumb
    // it into the tree while we are awaiting our get.
    // We'll get within ra_page from ra_line to ra_line_end.
    INFO_PRINT(("%i chpl_comm_start_get(%p, %i, %p, %i)\n",
                 (int) chpl_nodeID, page+(ra_line-ra_page), node, (void*) ra_line,
                 (int) (ra_line_end - ra_line)));

    memset(page+(ra_line-ra_page), /*local addr*/
           0,
           ra_line_end - ra_line /*size*/);

    if( entry ) {
      use_entry(cache, entry);
    } else {
      entry = make_entry(cache, node, ra_page, page, aout_entry);
    }

    // Set the valid lines
    set_valid_lines(entry->valid_lines,
                    (ra_line - ra_page) >> CACHELINE_BITS,
                    (ra_line_end - ra_line) >> CACHELINE_BITS);

    {
      // This will increment next request number so cache events are recorded.
      sn = cache->next_request_number;
      cache->next_request_number++;
    }

    // Set the minimum sequence number
    entry->min_sequence_number = seqn_min(entry->min_sequence_number, sn);

    // Update the last read location on a miss
    // (as long as there was not an intervening acquire)
    if( entry_after_acquire && sequential_readahead_length == 0 ) {
      cache->last_cache_miss_read_node = node;
      cache->last_cache_miss_read_addr = ra_line;
    }

    // Make sure that there is an available page for next time,
    // but do it without evicting entry (that we are working with).
    // This could happen if entry is the last element of Ain..
    ensure_free_page(cache, task_local, /* give_up_if_locked */ 1);

    // would copy the data out here, but not in mock get
  }

  if( VERIFY ) validate_cache(cache, task_local);

#ifdef DUMP
  printf("After mock_get cache is:\n");
  rdcache_print(cache);
#endif

  return all_in_cache;
}


static
void cache_invalidate_in_page(struct rdcache_s* cache,
                              chpl_cache_taskPrvData_t* task_local,
                              c_nodeid_t node, raddr_t raddr, size_t size)
{
  raddr_t ra_page;

  // ra_page = raddr of start of the page
  ra_page = round_down_to_mask(raddr, CACHEPAGE_MASK);

  while (1) {
    // Is the page in the tree?
    struct cache_entry_s* entry = lookup_entry(cache, node, ra_page);

    if (entry && entry->page) {
      if (!try_reserve_entry(cache, task_local, entry)) {
        // couldn't reserve entry - yield and try the lookup again
        TRACE_YIELD_PRINT(("%d: task %d cache %p yielding in invalidate\n",
                           chpl_nodeID, (int) chpl_task_getId(), cache));

        chpl_task_yield();

        TRACE_YIELD_PRINT(("%d: task %d cache %p back in invalidate\n",
                       chpl_nodeID, (int) chpl_task_getId(), cache));

        // try again
        continue;
      }

      // "lock"ed entry
      flush_entry(cache, task_local, entry,
                  FLUSH_INVALIDATE_REGION, raddr, size);

      // "unlock" the entry
      unreserve_entry(cache, task_local, entry);

    }

    // all cases other than the 'continue' case above should return now
    //  * no page was found
    //  * found page, locked it, and flushed it
    return;
  }
}

static
void cache_invalidate(struct rdcache_s* cache,
                      chpl_cache_taskPrvData_t* task_local,
                      c_nodeid_t node, raddr_t raddr, size_t size)
{
  raddr_t ra_first_page;
  raddr_t ra_last_page;
  raddr_t ra_next_page;
  raddr_t ra_page;
  raddr_t ra_page_end;
  raddr_t requested_start, requested_end, requested_size;

  DEBUG_PRINT(("invalidate from %i:%p len %i\n",
               (int) node, raddr, (int) size));

  if (chpl_nodeID == node) {
    // Do nothing if we're on the same node.
    return;
  }

  // first_page = raddr of start of first needed page
  ra_first_page = round_down_to_mask(raddr, CACHEPAGE_MASK);
  // last_page = raddr of start of last needed page
  ra_last_page = round_down_to_mask(raddr+size-1, CACHEPAGE_MASK);
  ra_next_page = ra_last_page + CACHEPAGE_SIZE;

  // Try to find it in the cache. Go through one page at a time.
  for( ra_page = ra_first_page;
       ra_page <= ra_last_page;
       ra_page += CACHEPAGE_SIZE ) {

    // We will need from ra_line to ra_line_end.
    ra_page_end = (ra_page==ra_last_page)?(ra_next_page):(ra_page+CACHEPAGE_SIZE);

    // Compute the portion of the page that was requested
    requested_start = raddr_max(raddr,ra_page);
    requested_end = raddr_min(raddr+size,ra_page_end);
    requested_size = requested_end - requested_start;

    // invalidate that portion of the page
    cache_invalidate_in_page(cache, task_local, node,
                             requested_start, requested_size);
  }
}

static
void cache_clean_dirty(struct rdcache_s* cache,
                       chpl_cache_taskPrvData_t* task_local)
{

  while (1) {
    struct dirty_entry_s* cur;
    struct cache_entry_s* victim;

    cur = cache->dirty_lru_head;
    // Dirty records with page entries are before free ones without
    if( ! cur || ! cur->entry ) break;

    victim = cur->entry;

    if (!try_reserve_entry(cache, task_local, victim)) {
      // couldn't reserve entry - yield and try the lookup again
      TRACE_YIELD_PRINT(("%d: task %d cache %p yielding in clean_dirty\n",
                         chpl_nodeID, (int) chpl_task_getId(), cache));

      chpl_task_yield();

      TRACE_YIELD_PRINT(("%d: task %d cache %p back in clean_dirty\n",
                         chpl_nodeID, (int) chpl_task_getId(), cache));

      continue;
    }

    // "lock"ed entry
    flush_entry(cache, task_local, victim,
                FLUSH_DO_CLEAR_DIRTY, 0, CACHEPAGE_SIZE);
    unreserve_entry(cache, task_local, victim);
  }
}

static
void wait_all(struct rdcache_s* cache)
{
  int index;
  cache_seqn_t sn;

  index = cache->pending_last_entry;
  if( index >= 0 ) {
    sn = cache->pending_sequence_numbers[index];
    wait_for(cache, sn);
  }
}



static struct rdcache_s* cache_create(void);

// We access the pthread-specific version of the cache
// using __thread declarations if possible, but we also
// use pthread_setspecific/pthread_key_create so that
// we can associate a destructor with it.
CHPL_TLS_DECL(struct rdcache_s*,cache_remote_data);
static pthread_key_t pthread_cache_info_key; // stores struct rdcache_s*

static
struct rdcache_s* tls_cache_remote_data(void) {
  struct rdcache_s *cache = CHPL_TLS_GET(cache_remote_data);
  if( ! cache && chpl_cache_enabled() ) {
    cache = cache_create();
    CHPL_TLS_SET(cache_remote_data, cache);
    pthread_setspecific(pthread_cache_info_key, cache);
  }
  return cache;
}

static
chpl_cache_taskPrvData_t* task_private_cache_data(void)
{
  chpl_task_infoRuntime_t* infoRuntime = chpl_task_getInfoRuntime();
  assert(infoRuntime);
  return &infoRuntime->comm_data.cache_data;
}

static
void destroy_pthread_local_cache(void* arg)
{
  struct rdcache_s* s = (struct rdcache_s*) arg;
  cache_destroy(s);
}

static
void chpl_cache_do_init(void)
{
  static int inited = 0;
  if( ! inited ) {
    // We will need some thread-local storage.
    // We create two versions: cache_remote_data stores
    // our pointer to the struct rd_cache_s* and is what
    // we normally use, with __thread or CHPL_TLS_GET which is
    // faster.
    CHPL_TLS_INIT(cache_remote_data);
    // The second key we never read but create so that we
    // can free the cache when the thread exits.
    pthread_key_create(&pthread_cache_info_key, &destroy_pthread_local_cache);
    inited = 1;
  }
}

// The implementation of functions in chpl-cache.h

void chpl_cache_init(void) {

  chpl_cache_warn_if_disabled();

  if( ! chpl_cache_enabled() ) {
    return;
  }

  //printf("CACHE IS ENABLED\n");
  chpl_cache_do_init();
}

void chpl_cache_exit(void)
{
  CHPL_TLS_DELETE(cache_remote_data);
}


void chpl_cache_fence(int acquire, int release, int ln, int32_t fn)
{
  if( acquire == 0 && release == 0 ) return;
  if( chpl_cache_enabled() ) {
    struct rdcache_s* cache = tls_cache_remote_data();
    chpl_cache_taskPrvData_t* task_local = task_private_cache_data();

    INFO_PRINT(("%i fence acquire %i release %i %s:%i\n", chpl_nodeID, acquire, release, fn, ln));

    TRACE_FENCE_PRINT(("%d: task %d in chpl_cache_fence(acquire=%i,release=%i)"
                       " on cache %p from %s:%d\n",
                       chpl_nodeID, (int) chpl_task_getId(), acquire, release,
                       cache, chpl_lookupFilename(fn), ln));

#ifdef DUMP
    DEBUG_PRINT(("%d: task %d before fence\n", chpl_nodeID, (int) chpl_task_getId()));
    chpl_cache_print();
#endif

    if( acquire ) {
      task_local->last_acquire = cache->next_request_number;
      cache->next_request_number++;
    }

    if( release ) {
      cache_clean_dirty(cache, task_local);
      wait_all(cache);
    }
#ifdef DUMP
    DEBUG_PRINT(("%d: task %d after fence\n", chpl_nodeID, (int) chpl_task_getId()));
    chpl_cache_print();
#endif
  }
  // Do nothing if cache is not enabled.
}

void chpl_cache_invalidate(c_nodeid_t node, void* raddr, size_t size,
                           int ln, int32_t fn)
{
  struct rdcache_s* cache = tls_cache_remote_data();
  chpl_cache_taskPrvData_t* task_local = task_private_cache_data();


  TRACE_PRINT(("%d: task %d in chpl_cache_invalidate %s:%d %d bytes at %d:%p\n",
               chpl_nodeID, (int)chpl_task_getId(),
               chpl_lookupFilename(fn), ln,
               (int)size, node, raddr, addr));

  cache_invalidate(cache, task_local, node, (raddr_t)raddr, size);
}

// If a transfer is large enough we should directly initiate it to avoid
// overheads of going through the cache
//
// This is not allowed to modify the cache
static inline
int size_merits_direct_comm(const struct rdcache_s* cache, size_t size)
{
  return size >= CACHEPAGE_SIZE;
}

void chpl_cache_comm_put(void* addr, c_nodeid_t node, void* raddr,
                         size_t size, int32_t commID, int ln, int32_t fn)
{
  struct rdcache_s* cache = tls_cache_remote_data();
  chpl_cache_taskPrvData_t* task_local = task_private_cache_data();
  int all_hits;

  if (size_merits_direct_comm(cache, size)) {
    cache_invalidate(cache, task_local, node, (raddr_t)raddr, size);
    chpl_comm_put(addr, node, raddr, size, commID, ln, fn);
    if (EXTRA_YIELDS) {
      TRACE_YIELD_PRINT(("%d: task %d cache %p yielding for chpl_comm_put\n",
                         chpl_nodeID, (int) chpl_task_getId(), cache));

      chpl_task_yield();

      TRACE_YIELD_PRINT(("%d: task %d cache %p back from chpl_comm_put\n",
                         chpl_nodeID, (int) chpl_task_getId(), cache));
    }
    return;
  }
  TRACE_PRINT(("%d: task %d in chpl_cache_comm_put %s:%d put %d bytes to %d:%p "
               "from %p\n",
               chpl_nodeID, (int)chpl_task_getId(), chpl_lookupFilename(fn), ln,
               (int)size, node, raddr, addr));
  chpl_comm_diags_verbose_rdma("put", node, size, ln, fn, commID);

#ifdef DUMP
  chpl_cache_print();
#endif

  all_hits = cache_put(cache, task_local,
                       addr, node, (raddr_t)raddr, size,
                       commID, ln, fn);

  if (size != 0) {
    if (all_hits)
      chpl_comm_diags_incr(cache_put_hits);
    else
      chpl_comm_diags_incr(cache_put_misses);
  }

  return;
}

void chpl_cache_comm_get(void *addr, c_nodeid_t node, void* raddr,
                         size_t size, int32_t commID, int ln, int32_t fn)
{
  //printf("get len %d node %d raddr %p\n", (int) len * elemSize, node, raddr);
  struct rdcache_s* cache = tls_cache_remote_data();
  chpl_cache_taskPrvData_t* task_local = task_private_cache_data();
  int all_hits;

  if (size_merits_direct_comm(cache, size)) {
    cache_invalidate(cache, task_local, node, (raddr_t)raddr, size);
    chpl_comm_get(addr, node, raddr, size, commID, ln, fn);
    if (EXTRA_YIELDS) {
      TRACE_YIELD_PRINT(("%d: task %d cache %p yielding for chpl_comm_get\n",
                         chpl_nodeID, (int) chpl_task_getId(), cache));

      chpl_task_yield();

      TRACE_YIELD_PRINT(("%d: task %d cache %p back from chpl_comm_get\n",
                         chpl_nodeID, (int) chpl_task_getId(), cache));
    }
    return;
  }
  TRACE_PRINT(("%d: task %d in chpl_cache_comm_get %s:%d get %d bytes from "
               "%d:%p to %p\n",
               chpl_nodeID, (int)chpl_task_getId(), chpl_lookupFilename(fn), ln,
               (int)size, node, raddr, addr));
  chpl_comm_diags_verbose_rdma("get", node, size, ln, fn, commID);

#ifdef DUMP
  chpl_cache_print();
#endif

  all_hits = cache_get(cache, task_local,
                       addr, node, (raddr_t)raddr, size,
                       0, commID, ln, fn);

  if (size != 0) {
    if (all_hits)
      chpl_comm_diags_incr(cache_get_hits);
    else
      chpl_comm_diags_incr(cache_get_misses);
  }

  return;
}

void chpl_cache_comm_prefetch(c_nodeid_t node, void* raddr,
                              size_t size, int32_t commID, int ln, int32_t fn)
{
  struct rdcache_s* cache = tls_cache_remote_data();
  chpl_cache_taskPrvData_t* task_local = task_private_cache_data();

  TRACE_PRINT(("%d: in chpl_cache_comm_prefetch\n", chpl_nodeID));

  chpl_comm_diags_verbose_rdma("prefetch", node, size, ln, fn, commID);

  // Always use the cache for prefetches.
  cache_get(cache, task_local,
            /* addr */ NULL, node, (raddr_t)raddr, size,
            /* sequential_readahead_length */ 0,
            CHPL_COMM_UNKNOWN_ID, ln, fn);

  // TODO: record prefetches somewhere in diagnostic counters
}

struct cache_strd_callback_ctx {
  struct rdcache_s* cache;
  chpl_cache_taskPrvData_t* task_local;
};

static
void strd_invalidate_fn(void* addr,
                        int32_t node,
                        void* raddr,
                        size_t size,
                        void* ctxv,
                        int32_t commID,
                        int ln,
                        int32_t fn)
{

  struct cache_strd_callback_ctx* ctx = (struct cache_strd_callback_ctx*) ctxv;

  struct rdcache_s* cache = ctx->cache;
  chpl_cache_taskPrvData_t* task_local = ctx->task_local;

  cache_invalidate(cache, task_local, node, (raddr_t)raddr, size);
}

static
void strd_invalidate(void *addr, void *dststr, c_nodeid_t node,
                     void *raddr, void *srcstr, void *count,
                     int32_t strlevels, size_t elemSize,
                     int32_t commID, int ln, int32_t fn) {

  struct rdcache_s* cache = tls_cache_remote_data();
  chpl_cache_taskPrvData_t* task_local = task_private_cache_data();

  struct cache_strd_callback_ctx ctx;
  ctx.cache = cache;
  ctx.task_local = task_local;
  strd_common_call(addr, dststr, node,
                   raddr, srcstr, count, strlevels, elemSize,
                   &ctx, &strd_invalidate_fn, commID, ln, fn);
}

#define STRIDED_INVALIDATE_ALL 0

void chpl_cache_comm_get_strd(void *addr, void *dststr, c_nodeid_t node,
                              void *raddr, void *srcstr, void *count,
                              int32_t strlevels, size_t elemSize,
                              int32_t commID, int ln, int32_t fn) {
  TRACE_PRINT(("%d: in chpl_cache_comm_get_strd\n", chpl_nodeID));

  if (STRIDED_INVALIDATE_ALL) {
    // do a full fence - so that:
    // 1) any pending writes are completed (in case they were to the
    //    same location handled by the strided get)
    // 2) the cache does not have older values than what we're getting now
    // Alternatively, we could invalidate the requested regions.
    // Alternatively, the strided get could be done through the cache
    // system. This is just the current (possibly temporary) solution.
    chpl_cache_fence(1, 1, ln, fn);
  } else {
    strd_invalidate(addr, dststr, node,
                    raddr, srcstr, count,
                    strlevels, elemSize,
                    commID, ln, fn);
  }
  // do the strided get.
  chpl_comm_get_strd(addr, dststr, node, raddr, srcstr, count, strlevels,
                     elemSize, commID, ln, fn);
  if (EXTRA_YIELDS) {
#ifdef TRACE_YIELDS
    struct rdcache_s* cache = tls_cache_remote_data();
#endif
    TRACE_YIELD_PRINT(("%d: task %d cache %p yielding for chpl_comm_get_strd\n",
                      chpl_nodeID, (int) chpl_task_getId(), cache));

    chpl_task_yield();

    TRACE_YIELD_PRINT(("%d: task %d cache %p back from chpl_comm_get_strd\n",
                       chpl_nodeID, (int) chpl_task_getId(), cache));
  }
}
void chpl_cache_comm_put_strd(void *addr, void *dststr, c_nodeid_t node,
                              void *raddr, void *srcstr, void *count,
                              int32_t strlevels, size_t elemSize,
                              int32_t commID, int ln, int32_t fn) {
  TRACE_PRINT(("%d: in chpl_cache_comm_put_strd\n", chpl_nodeID));

  if (STRIDED_INVALIDATE_ALL) {
    // do a full fence - so that:
    // 1) any pending writes are completed (in case they were to the
    //    same location handled by the strided put and would
    //    complete in the wrong order)
    // 2) the cache does not keep older values from before the put.
    // Alternatively, the strided put could be done through the cache
    // system. This is just the current (possibly temporary) solution.
    chpl_cache_fence(1, 1, ln, fn);
  } else {
    strd_invalidate(addr, dststr, node,
                    raddr, srcstr, count,
                    strlevels, elemSize,
                    commID, ln, fn);
  }
  // do the strided put.
  chpl_comm_put_strd(addr, dststr, node, raddr, srcstr, count, strlevels,
                     elemSize, commID, ln, fn);
  if (EXTRA_YIELDS) {
#ifdef TRACE_YIELDS
    struct rdcache_s* cache = tls_cache_remote_data();
#endif
    TRACE_YIELD_PRINT(("%d: task %d cache %p yielding for chpl_comm_put_strd\n",
                       chpl_nodeID, (int) chpl_task_getId(), cache));

    chpl_task_yield();

    TRACE_YIELD_PRINT(("%d: task %d cache %p back from chpl_comm_put_strd\n",
                       chpl_nodeID, (int) chpl_task_getId(), cache));
  }
}

//
// Directly initiate unordered comm, invalidate any pending updates to
// overlapping regions beforehand.
//
void chpl_cache_comm_put_unordered(void* addr, c_nodeid_t node, void* raddr,
                                   size_t size, int32_t commID, int ln, int32_t fn)
{
  struct rdcache_s* cache = tls_cache_remote_data();
  chpl_cache_taskPrvData_t* task_local = task_private_cache_data();
  cache_invalidate(cache, task_local, node, (raddr_t)raddr, size);
  chpl_comm_put_unordered(addr, node, raddr, size, commID, ln, fn);
  if (EXTRA_YIELDS) {
    TRACE_YIELD_PRINT(("%d: task %d cache %p yielding for put_unordered\n",
                      chpl_nodeID, (int) chpl_task_getId(), cache));

    chpl_task_yield();

    TRACE_YIELD_PRINT(("%d: task %d cache %p back from put_unordered\n",
                      chpl_nodeID, (int) chpl_task_getId(), cache));
  }
}

void chpl_cache_comm_get_unordered(void *addr, c_nodeid_t node, void* raddr,
                                   size_t size, int32_t commID, int ln, int32_t fn)
{
  struct rdcache_s* cache = tls_cache_remote_data();
  chpl_cache_taskPrvData_t* task_local = task_private_cache_data();
  cache_invalidate(cache, task_local, node, (raddr_t)raddr, size);
  chpl_comm_get_unordered(addr, node, raddr, size, commID, ln, fn);
  if (EXTRA_YIELDS) {
    TRACE_YIELD_PRINT(("%d: task %d cache %p yielding for get_unordered\n",
                       chpl_nodeID, (int) chpl_task_getId(), cache));

    chpl_task_yield();

    TRACE_YIELD_PRINT(("%d: task %d cache %p back from put_unordered\n",
                      chpl_nodeID, (int) chpl_task_getId(), cache));
  }
}


void chpl_cache_comm_getput_unordered(c_nodeid_t dstnode, void* dstaddr,
                                      c_nodeid_t srcnode, void* srcaddr,
                                      size_t size, int32_t commID,
                                      int ln, int32_t fn)
{
  struct rdcache_s* cache = tls_cache_remote_data();
  chpl_cache_taskPrvData_t* task_local = task_private_cache_data();
  cache_invalidate(cache, task_local, srcnode, (raddr_t)srcaddr, size);
  cache_invalidate(cache, task_local, dstnode, (raddr_t)dstaddr, size);
  chpl_comm_getput_unordered(dstnode, dstaddr, srcnode, srcaddr, size, commID, ln, fn);
  if (EXTRA_YIELDS) {
    TRACE_YIELD_PRINT(("%d: task %d cache %p yielding for getput_unordered\n",
                       chpl_nodeID, (int) chpl_task_getId(), cache));

    chpl_task_yield();

    TRACE_YIELD_PRINT(("%d: task %d cache %p back from put_unordered\n",
                      chpl_nodeID, (int) chpl_task_getId(), cache));
  }
}

void chpl_cache_comm_getput_unordered_task_fence(void)
{
  chpl_comm_getput_unordered_task_fence();
}

// This is for debugging.
void chpl_cache_print(void)
{
  struct rdcache_s* cache = tls_cache_remote_data();
  chpl_cache_taskPrvData_t* task_local = task_private_cache_data();
  printf("%d: cache dump last acquire %i\n", chpl_nodeID, (int) task_local->last_acquire);
  rdcache_print(cache);
}

// This is for debugging.
void chpl_cache_assert_released(void)
{
  struct rdcache_s* cache = tls_cache_remote_data();
  struct dirty_entry_s* cur;
  cache_seqn_t sn;
  int index;

  cur = cache->dirty_lru_head;
  if ( cur && cur->entry )
    assert(0); //dirty entry

  index = cache->pending_last_entry;

  if( index >= 0 ) {
    sn = cache->pending_sequence_numbers[index];
    if( sn <= cache->completed_request_number ) return;
    assert(0); //pending request
  }
}

// This is for debugging
void chpl_cache_print_stats(void) {
  struct rdcache_s* cache = tls_cache_remote_data();
  int n_used_slots = 0;
  int n_full_slots = 0;
  int n_bottom_entries = 0;
  int n_colliding_slots = 0;
  int n_full_subslots = 0;
  int n_subslots = 0;
  int table_slots = 1 << cache->table_bits;
  for (int i = 0; i < table_slots; i++) {
    struct cache_table_slot_s* slot = &cache->table[i];
    int full_entries_this_slot = 0;
    int this_slot_collides = 0;
    for (int j = 0; j < TABLE_ENTRIES_PER_SLOT; j++) {
      struct cache_table_entry_s* entry = &slot->m[j];
      struct cache_entry_s* bottom_cur = offset_to_entry(cache, entry->offset);
      int bottom_entries_this_entry = 0;

      if (entry->offset != 0)
        full_entries_this_slot++;

      while(bottom_cur) {
        bottom_entries_this_entry++;
        bottom_cur = (struct cache_entry_s*) bottom_cur->base.next;
      }
      n_bottom_entries += bottom_entries_this_entry;
      if (bottom_entries_this_entry > 1)
        this_slot_collides = 1;
      n_subslots++;
    }
    if (this_slot_collides)
      n_colliding_slots++;
    if (full_entries_this_slot == TABLE_ENTRIES_PER_SLOT)
      n_full_slots++;
    if (full_entries_this_slot > 0)
      n_used_slots++;
    n_full_subslots += full_entries_this_slot;
  }

  printf("%d: task %d cache statistics "
         "ain=%i/%i "
         "aout=%i/%i am=%i "
         "table=(%i lists/%i full/%i used/%i slots and %i/%i sub-slots) "
         "entries=%i/%i\n",
         chpl_nodeID, (int) chpl_task_getId(),
         cache->ain_current, cache->ain_max,
         cache->aout_current, cache->aout_max,
         cache->am_current,
         n_colliding_slots, n_full_slots, n_used_slots, table_slots,
         n_full_subslots, n_subslots,
         n_bottom_entries, cache->max_entries);
}

// Returns 1 if the data was already cached
int chpl_cache_mock_get(c_nodeid_t node, uint64_t raddr, size_t size)
{
  struct rdcache_s* cache = tls_cache_remote_data();
  int ret;

  if (!chpl_cache_enabled())
    chpl_internal_error("chpl_cache_mock_get called without --cache-remote");

  chpl_cache_taskPrvData_t* task_local = task_private_cache_data();
  TRACE_PRINT(("%d: task %d in chpl_cache_mock_get from %d:%p\n",
               chpl_nodeID, (int)chpl_task_getId(), node, (void*)raddr));

#ifdef DUMP
  chpl_cache_print();
#endif

  ret = mock_get(cache, task_local, node, (raddr_t)raddr, size,
                 task_local->last_acquire,
                 0, 0, 0, 0);

  return ret;
}


#endif
// end ifdef HAS_CHPL_CACHE_FNS
