/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
#include "chpl-linefile-support.h"
#include "sys.h" // sys_page_size()
#include "chpl-comm-compiler-macros.h"
#include "chpl-comm-no-warning-macros.h" // No warnings for chpl_comm_get etc.
#include <string.h> // memcpy, memset, etc.
#include <assert.h>


#ifdef HAS_CHPL_CACHE_FNS

// This is now just set at compile time.
//int CHPL_CACHE_REMOTE = 1;
#define VERIFY 0

// We try to auto-size the cache so that we
// can have CACHE_PAGES_PER_NODE cache pages per locale, but we
// do so within the below bounds.
#define CACHE_PAGES_PER_NODE 4
#define MIN_CACHE_DATA_SIZE (1024*1024)
#define MAX_CACHE_DATA_SIZE (256*1024*1024)

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

//#define TIME
//#define TRACE
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


#ifdef TRACE
#define TRACE_PRINT(x) printf x
#else
#define TRACE_PRINT(x) do {} while(0)
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



// ----------  SUPPORT FUNCTIONS 
#include "chpl-cache-support.c"




// Forward Declarations.
struct rdcache_s;


//////////////// REMOTE DATA CACHE IMPLEMENTATION ////////////////////

/*     (big endian diagram)

   |            64-bit address ^ (node_number << 32)                    |
   +---------------------------------------------------------------------+
   |    17 bits        | 10 bits  |     17 bits       | 10 bits  |10 bits|
   +------------------------------+------------------------------+-------+
         top half                 |      bottom half             |

   The structure defined in this file is a two-level hashtable representing
   memory, designed to be indexed by 64-bit addresses.  Given a 64-bit
   address, use the 10 bits from the "top half" of the address to figure
   out which top-level bucket to examine.  Each bucket contains a linked
   list, each entry of which is a top_index_entry.  The full 27 bits of the
   "top half" are matched with the entries in the linked list to find the
   correct one.

   Once the appropriate top_index_entry has been located, follow its
   bottom_index element to another hash table organized in the same way.
   Searching the linked list in the appropriate bucket in the bottom_index
   array will eventually lead to a bottom_index_entry which contains a 64
   byte cacheline, and we use the low-order 6 bits to index into this
   cacheline.  There is a bitmask tracking the validity of each byte in
   the cacheline.
   
   Attempts to read a byte from the cache which is not valid results in
   failure.
*/

#define TOP_BITS 10
#define BOTTOM_BITS 10
#define OTHER_BITS ((64-TOP_BITS-BOTTOM_BITS-CACHEPAGE_BITS)/2)
#define HALF_BITS (TOP_BITS+OTHER_BITS)

#define TOP_SIZE (1 << TOP_BITS)
#define BOTTOM_SIZE (1 << BOTTOM_BITS)
#define HALF_SIZE (1L << HALF_BITS)

// How many uint64_t words do we need to create a bitmask for CACHEPAGE_SIZE?
// Divide # bytes in cache by 64, rounding up.
#define CACHEPAGE_BITMASK_WORDS ((CACHEPAGE_SIZE+63)/64)

// How many cache lines per cache page?
#define CACHE_LINES_PER_PAGE (CACHEPAGE_SIZE/CACHELINE_SIZE)

// How many uint64_t words do we need to create a bitmask for CACHE_LINES_PER_PAGE
// ie, a mask recording a bit per cache line?
#define CACHE_LINES_PER_PAGE_BITMASK_WORDS (((CACHEPAGE_SIZE/CACHELINE_SIZE)+63)/64)

struct cache_entry_base_s {
  uint32_t index_bits;
  c_nodeid_t node;
  struct cache_entry_base_s* next; // hashtable collision chaining/freelist.
};

struct page_list_s {
  struct page_list_s* next;
  unsigned char* page;
};

// this data structure manages dirty pages. It is allocated separately
// from cache entries, so the majority of pages (which are clean) don't
// have the big bitmap.
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

// Storing a remote address (node number is separate).
typedef uintptr_t raddr_t;
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

// used to be bottom_index_entry.
struct cache_entry_s {
  struct cache_entry_base_s base; // contains what we hashed to...
  raddr_t raddr; // cached data is for (base.node,raddr), aligned to CACHE_PAGESIZE
  // Queue information. This entry could be in Ain, Aout, or Am queues.
  int queue;
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
  // If there are dirty bits in this page, what sequence number did
  // we promise to use in order to complete them?
  //cache_seqn_t dirty_sequence_number;
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
/*
static int count_valid_lines_before(uint64_t* valid, uintptr_t at)
{
  return count_valid_before(valid, at, CACHE_LINES_PER_PAGE_BITMASK_WORDS);
}
static int count_valid_lines_at_after(uint64_t* valid, uintptr_t at)
{
  return count_valid_at_after(valid, at, CACHE_LINES_PER_PAGE_BITMASK_WORDS);
}
*/

struct top_entry_s {
  struct cache_entry_base_s base; // contains what we hashed to...
  size_t num_entries;
  struct cache_entry_s* bottom_index[BOTTOM_SIZE];
};

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
  int max_top_nodes;

  // Free pages
  struct page_list_s* free_pages_head; // singly-linked list
  // Free page list entries (all page pointers should be NULL)
  struct page_list_s* free_page_list_entries_head; // singly-linked list
  // Free nodes
  struct cache_entry_base_s* free_entries_head; // singly-linked list with base.next

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

  // space for mid-level entries
  int max_top_entries;
  struct cache_entry_base_s* free_top_nodes_head; // a linked list.

  // The entry into the 'pointer tree' hashtable structure.
  struct top_entry_s* top_index_list[TOP_SIZE];
};

static void validate_cache(struct rdcache_s* tree);


static
struct rdcache_s* cache_create(void) {
  struct rdcache_s* c;
  int cache_pages;
  int ain_pages, aout_pages;
  int dirty_pages;
  int top_entries;
  int i;
  int n_entries;
  struct top_entry_s* top_nodes = NULL;
  struct page_list_s* page_list_entries = NULL;
  struct cache_entry_s *entries = NULL;
  struct dirty_entry_s *dirty_nodes = NULL;
  uintptr_t offset;

  size_t total_size = 0;
  size_t allocated_size = 0;
  unsigned int pending_len = MAX_PENDING; 
  unsigned char* buffer;
  unsigned char* pages;

  cache_pages = CACHE_PAGES_PER_NODE * chpl_numNodes;
  if( cache_pages < MIN_CACHE_DATA_SIZE/CACHEPAGE_SIZE )
    cache_pages = MIN_CACHE_DATA_SIZE/CACHEPAGE_SIZE;
  if( cache_pages > MAX_CACHE_DATA_SIZE/CACHEPAGE_SIZE )
    cache_pages = MAX_CACHE_DATA_SIZE/CACHEPAGE_SIZE;

  ain_pages = cache_pages / 4; // 2Q: "Kin should be 25% of page slots"
  aout_pages = cache_pages / 2; // 2Q: "Kout should hold identifiers for as
                                // many pages as would fit in 50% of the
                                // buffer"
  // How many pages can be dirty at once?
  dirty_pages = 16 + cache_pages / 64; 
  // How many mid-level elements can we have in our tree? Note each is 8k in the current config..
  top_entries = cache_pages / 16;
  // How many cache entries do we need? 
  n_entries = cache_pages + aout_pages;

  total_size += sizeof(struct rdcache_s);
  total_size += sizeof(struct page_list_s) * cache_pages;
  total_size += sizeof(struct cache_entry_s) * n_entries;
  total_size += sizeof(struct dirty_entry_s) * dirty_pages;
  total_size += sizeof(chpl_comm_nb_handle_t) * pending_len;
  total_size += sizeof(cache_seqn_t) * pending_len;
  total_size += sizeof(struct top_entry_s) * top_entries;
  // We allocate an extra page for alignment
  total_size += CACHEPAGE_SIZE + CACHEPAGE_SIZE * cache_pages;

  // Now, allocate it all in one go.
  buffer = chpl_malloc(total_size);
  allocated_size = total_size;

  // Now divvy up the portions...
  total_size = 0;
  c = (struct rdcache_s*) (buffer + total_size);
  total_size += sizeof(struct rdcache_s);
  // page list entries
  page_list_entries = (struct page_list_s*) (buffer + total_size);
  total_size += sizeof(struct page_list_s) * cache_pages;
  // entries
  entries = (struct cache_entry_s*) (buffer + total_size);
  total_size += sizeof(struct cache_entry_s) * n_entries;
  // dirty entries
  dirty_nodes = (struct dirty_entry_s*) (buffer + total_size);
  total_size += sizeof(struct dirty_entry_s) * dirty_pages;
  // and the pending data area
  c->pending = (chpl_comm_nb_handle_t*) (buffer + total_size);
  total_size += sizeof(chpl_comm_nb_handle_t) * pending_len;
  // and the pending sequence numbers
  c->pending_sequence_numbers = (cache_seqn_t*) (buffer + total_size);
  total_size += sizeof(cache_seqn_t) * pending_len;
  // and the top entries
  top_nodes = (struct top_entry_s*) (buffer + total_size);
  total_size += sizeof(struct top_entry_s) * top_entries;
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
  c->max_top_nodes = top_entries;

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
    struct cache_entry_base_s* next;
    if( i + 1 < n_entries ) next = &entries[i+1].base;
    else next = NULL;
    entries[i].base.next = next;
    entries[i].queue = QUEUE_FREE;
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

  c->max_top_entries = top_entries;
  // already set top_nodes
  // Set up top_nodes in a linked list on the free list.
  c->free_top_nodes_head = &top_nodes[0].base;
  for( i = 0; i < top_entries; i++ ) {
    struct cache_entry_base_s* next;
    if( i + 1 < top_entries ) next = &top_nodes[i+1].base;
    else next = NULL;
    top_nodes[i].base.next = next;
  }

  // clear top_index_list.
  memset(&c->top_index_list[0], 0, sizeof(struct top_entry_s*) * TOP_SIZE);

  if( VERIFY ) validate_cache(c);

  return c;
}

static
void cache_destroy(struct rdcache_s *cache) {
  chpl_free(cache);
}


static
void cache_entry_print(struct cache_entry_s* entry, const char* prefix, int print_data)
{
  int i,j,line;
  int valid;
  int valid_line;
  int dirty;
  printf("%scache entry %p index_bits %x node %i next %p\n",
         prefix,
         entry, entry->base.index_bits, entry->base.node, entry->base.next);
  printf("%sraddr %p queue %i readahead_skip %i readahead_len %i next %p prev %p page %p\n",
         prefix, (void*) entry->raddr, entry->queue, (int) entry->readahead_skip, (int) entry->readahead_len, entry->next, entry->prev, entry->page);
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
    cache_entry_print(entry, "    ain ", 1);
  }
  printf("  Aout:\n");
  for( entry = cache->aout_head; entry; entry = entry->next ) {
    cache_entry_print(entry, "   aout ", 1);
  }
  printf("  Am:\n");
  for( entry = cache->am_lru_head; entry; entry = entry->next ) {
    cache_entry_print(entry, "     am ", 1);
  }

  fflush(stdout);
}

static
void validate_cache(struct rdcache_s* tree);

static
int get_top_bits(raddr_t raddr, int32_t node) {
  uint64_t val;

  val = raddr;
  val = val >> (HALF_BITS + CACHEPAGE_BITS);
  val ^= (node & 0xaaaaaaaa); // spread node variation between top and bottom
  val &= (TOP_SIZE-1);

  return (int)val;
}

static
int get_bottom_bits(raddr_t raddr, int32_t node) {
  uintptr_t val;
  val = (raddr >> CACHEPAGE_BITS) & (BOTTOM_SIZE-1);
  val ^= (node & 0x55555555); // spread node variation between top and bottom
  return (int) val;
}

static
uint32_t get_high_bits(raddr_t raddr) {
  uint64_t val = raddr;
  return (val >> (HALF_BITS + CACHEPAGE_BITS)) & (HALF_SIZE-1);
}

static
uint32_t get_low_bits(raddr_t raddr) {
  return (raddr >> CACHEPAGE_BITS) & (HALF_SIZE-1);
}

static
struct cache_entry_base_s* list_search(struct cache_entry_base_s *head, 
    uint32_t target, int32_t node, struct cache_entry_base_s** prev) {
  struct cache_entry_base_s *ptr;
  struct cache_entry_base_s *prv = NULL;

  ptr = head;
  while(ptr) {
    if (ptr->index_bits == target && ptr->node == node) {
      if( prev ) *prev = prv;
      return ptr;
    }

    prv = ptr;
    ptr = ptr->next;
  }

  if( prev ) *prev = NULL;
  return NULL;
}

static
struct top_entry_s* top_list_search(struct top_entry_s* head,
    uint32_t target, int32_t node, struct cache_entry_base_s ** prev) {
  return (struct top_entry_s*) list_search((struct cache_entry_base_s*)head,
                                           target, node,
                                           (struct cache_entry_base_s**)prev);
}

static
struct cache_entry_s* bottom_list_search(struct cache_entry_s* head,
    uint32_t target, int32_t node, struct cache_entry_base_s ** prev) {
  return (struct cache_entry_s*) list_search((struct cache_entry_base_s*)head,
                                             target, node,
                                             (struct cache_entry_base_s**)prev);
}

static
void top_entry_free(struct rdcache_s* tree, struct top_entry_s* victim)
{
  struct cache_entry_base_s* b = &victim->base;
  // Put the newly free top node into the free list
  SINGLE_PUSH_HEAD(tree, b, free_top_nodes);
}


// Removes 'element' from the tree.
// Does not wait for any operations to complete or free any cache entries
// (it will return unused top_entries to the free list)
static
void tree_remove(struct rdcache_s* tree, struct cache_entry_s* element)
{
  struct cache_entry_base_s *prev;
  struct cache_entry_base_s *prev_top;
  struct top_entry_s **head;
  struct top_entry_s *match;
  struct cache_entry_s **bottom;
  struct cache_entry_s *bottom_match;
  int top_idx, bottom_idx;
  uint32_t high_bits, low_bits;
  int32_t node = element->base.node;

  DEBUG_PRINT(("%d: Removing %p element %p\n", chpl_nodeID,
               (void*) element->raddr, element));

  top_idx = get_top_bits(element->raddr, element->base.node);
  bottom_idx = get_bottom_bits(element->raddr, element->base.node);
  high_bits = get_high_bits(element->raddr);
  low_bits = get_low_bits(element->raddr);

  head = &tree->top_index_list[top_idx];

  match = top_list_search(*head, high_bits, node, &prev_top);

  assert( match );
  assert(match->bottom_index);

  prev = NULL;
  bottom = &match->bottom_index[bottom_idx];
  bottom_match = bottom_list_search(*bottom, low_bits, node, &prev);

  assert( bottom_match );
  assert( bottom_match == element );

  // Remove bottom_match (== element) from the list.
  if( prev ) {
    prev->next = bottom_match->base.next;
  } else {
    // no prev means we are the list head.
    *bottom = (struct cache_entry_s*) bottom_match->base.next;
  }

  match->num_entries--; // net loss of one element.

  if( *bottom == NULL ) {
    // If there are no entries left in the bottom bucket, and
    // there are no entries left in the top bucket, remove that
    // top bucket.
    if( match->num_entries == 0 ) {
      // Remove match from the list.
      if( prev_top ) {
        prev_top->next = match->base.next;
      } else {
        // No prev means we are the list head.
        *head = (struct top_entry_s*) match->base.next;
      }
      // Put the newly free top node into the free list
      top_entry_free(tree, match);
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

// For the region of this page in raddr,len, we complete any pending/not
// started operations that possibly overlap with that region.
// If FLUSH_EVICT or FLUSH_INVALIDATE_PAGE is set, we will ignore the region.
static
void flush_entry(struct rdcache_s* cache, struct cache_entry_s* entry, int op,
                 raddr_t raddr, int32_t len_in);

static
void aout_evict(struct rdcache_s* cache)
{
  struct cache_entry_s* z;
  struct cache_entry_base_s* entry;

  z = cache->aout_tail;

  if( !z ) return;

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
void ain_evict(struct rdcache_s* cache, struct cache_entry_s* dont_evict_me)
{
  struct cache_entry_s* y;

  y = cache->ain_tail;

  if( ! y ) return;
  if( y == dont_evict_me ) {
    // Remove dont_evict_me from the tail of ain
    DOUBLE_REMOVE_TAIL(cache, ain);
    // Evict the new tail element
    ain_evict(cache, NULL);
    // Put dont_evict_me back on the tail.
    DOUBLE_PUSH_TAIL(cache, dont_evict_me, ain);
  }

#ifdef DEBUG
  DEBUG_PRINT(("Ain is evicting entry for raddr %p\n", (void*) y->raddr));
  cache_entry_print( y, " ain evict ", 1);
  rdcache_print(cache);
#endif

  // If the entry in Ain has any pending/dirty requests, we must
  // immediately wait for them to complete, before we modify the contents
  // of Ain in any way (or reuse the associated page).
  flush_entry(cache, y, FLUSH_EVICT, 0, CACHEPAGE_SIZE);

  DOUBLE_REMOVE_TAIL(cache, ain);
  cache->ain_current--;

  y->queue = QUEUE_AOUT;

  // Since we are kicking entry off of Ain, we have to add it to Aout.
  DOUBLE_PUSH_HEAD(cache, y, aout);
  cache->aout_current++;

  if( cache->aout_current > cache->aout_max ) {
    // Remove the tail element from aout.
    aout_evict(cache);
  }
}

 
static
void am_evict(struct rdcache_s *cache, struct cache_entry_s* dont_evict_me) {
  struct cache_entry_s *y;
  struct cache_entry_base_s* entry;
    
  y = cache->am_lru_tail;

  if( !y ) return;
  if( y == dont_evict_me ) {
    // Remove dont_evict_me from the tail of am
    DOUBLE_REMOVE_TAIL(cache, am_lru);
    // Evict the new tail element
    am_evict(cache, NULL);
    // Put dont_evict_me back on the tail.
    DOUBLE_PUSH_TAIL(cache, dont_evict_me, am_lru);
  }

  // If the entry in Am has any pending/dirty requests, we must
  // immediately wait for them to complete, before we modify the contents
  // of Ain in any way (or reuse the associated page).
  flush_entry(cache, y, FLUSH_EVICT, 0, CACHEPAGE_SIZE);

  DOUBLE_REMOVE_TAIL(cache, am_lru);
  cache->am_current--;

  // Remove this entry in Am from the pointer tree.
  tree_remove(cache, y);

  y->queue = QUEUE_FREE;

  // Add y to the free list
  entry = &y->base;
  SINGLE_PUSH_HEAD(cache, entry, free_entries);
}
 

static
struct top_entry_s* top_entry_allocate(struct rdcache_s* tree)
{
  struct cache_entry_base_s* ret;

  while( ! tree->free_top_nodes_head ) {
    // If there's nothing in our free list, we have to evict something
    // from the cache.
    // Take turns evicting from Aout, Ain, and Am.

    // Evict from Aout 2x (since evicting Ain adds to Aout)
    aout_evict(tree);
    if( tree->free_top_nodes_head ) break; 
    aout_evict(tree);
    if( tree->free_top_nodes_head ) break; 

    // Evict from Ain (will add an entry to aout)
    ain_evict(tree, NULL);
    if( tree->free_top_nodes_head ) break; 

    // Evict from Am
    am_evict(tree, NULL);
    if( tree->free_top_nodes_head ) break; 
  }

  // Remove the top element from the list.
  assert(tree->free_top_nodes_head);
  ret = tree->free_top_nodes_head;
  SINGLE_POP_HEAD(tree, free_top_nodes);
  ret->next = NULL;
  return (struct top_entry_s*) ret;
}

static
void reclaim(struct rdcache_s* cache, struct cache_entry_s* dont_evict_me)
{
  // This is like 'reclaimfor' in the 2Q paper
  // if the number of elements in Ain > max
  if( cache->ain_current > cache->ain_max ) {
    // Page out the tail of Ain (and record it in Aout)
    // ain_evict will also evict from aout if necessary.
    ain_evict(cache, dont_evict_me);
  } else {
    // otherwise
    // page out the tail of Am, call it Y
    // do not put it on Aout, as it hasn't been accessed recently.
    am_evict(cache, dont_evict_me);
  }
}

static
struct cache_entry_s* allocate_entry(struct rdcache_s* cache)
{
  struct cache_entry_s* ret;

  //printf("alloc entry ain %i aout %i am %i\n", 
  //       cache->ain_current, cache->aout_current, cache->am_current);

  // Make sure we have a free entry..
  if( ! cache->free_entries_head ) reclaim(cache, NULL);

  ret = (struct cache_entry_s*) cache->free_entries_head;

  // Remove it from the single-linked list.
  SINGLE_POP_HEAD(cache, free_entries);

  return ret; 
}


// Make sure there is a free page on the free list,
// but don't evict a page with an ongoing operation..
static
void ensure_free_page(struct rdcache_s* cache, struct cache_entry_s* dont_evict_me)
{
  // If there are free page slots, then use a free page slot.
  if( cache->free_pages_head ) {
    return;
  }

  reclaim(cache, dont_evict_me);

  assert( cache->free_pages_head );
}

static
unsigned char* allocate_page(struct rdcache_s* cache)
{
  struct page_list_s* page_list_entry;
  unsigned char* ret;

  if( ! cache->free_pages_head ) ensure_free_page(cache, NULL);

  page_list_entry = cache->free_pages_head;
  SINGLE_POP_HEAD(cache, free_pages);

  ret = page_list_entry->page;
  page_list_entry->page = NULL;
  SINGLE_PUSH_HEAD(cache, page_list_entry, free_page_list_entries);

  return ret;
}

static
void ensure_free_dirty(struct rdcache_s* cache)
{
  assert( cache->dirty_lru_tail );

  if( cache->dirty_lru_tail->entry ) {
    flush_entry(cache, cache->dirty_lru_tail->entry, FLUSH_DO_PAGE|FLUSH_DO_CLEAR_DIRTY,
                0, CACHEPAGE_SIZE);
  }
}


static struct dirty_entry_s* allocate_dirty(struct rdcache_s* cache,
                                            struct cache_entry_s* for_entry)
{
  struct dirty_entry_s* dirty;

  // We never remove from dirty_lru, just reorder...
  assert( cache->dirty_lru_tail );

  if( cache->dirty_lru_tail->entry ) ensure_free_dirty(cache);

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

  if( fifo_circleb_isfull(cache->pending_first_entry, cache->pending_last_entry, cache->pending_len) ) {
    index = cache->pending_first_entry;
    wait_sn = cache->pending_sequence_numbers[index];
    wait_for(cache, wait_sn);
    // We should have completed at least the operation at index.
    assert( cache->pending[index] == NULL );
  }

  sn = cache->next_request_number;
  cache->next_request_number++;

  fifo_circleb_push(&cache->pending_first_entry, &cache->pending_last_entry, cache->pending_len);
  index = cache->pending_last_entry;
  cache->pending[index] = handle;
  cache->pending_sequence_numbers[index] = sn;
 
  DEBUG_PRINT(("in pending_push added pending[%i]=%p sn=%i\n",
               index, (void*) handle, (int) sn));

  return sn;
}

static
struct cache_entry_s* find_in_tree(struct rdcache_s* tree,
                                   c_nodeid_t node, raddr_t raddr)
{
  int top_idx, bottom_idx;
  uint32_t high_bits;
  uint32_t low_bits;
  struct top_entry_s **head, *top_match;
  struct cache_entry_s **bottom, *bottom_match;
  struct cache_entry_base_s* bottom_prev;

  assert(raddr != 0);

  top_idx = get_top_bits(raddr, node);
  bottom_idx = get_bottom_bits(raddr, node);
  high_bits = get_high_bits(raddr);
  low_bits = get_low_bits(raddr);

  head = &tree->top_index_list[top_idx];
  top_match = top_list_search(*head, high_bits, node, NULL);
  if (!top_match) return NULL;
  bottom = &top_match->bottom_index[bottom_idx];
  bottom_match = bottom_list_search(*bottom, low_bits, node, &bottom_prev);
  return bottom_match;
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
int validate_queue(struct rdcache_s* tree, struct cache_entry_s* head, struct cache_entry_s* tail, int queue)
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
      found = find_in_tree(tree, cur->base.node, cur->raddr);
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
    forward_count++;
  }
  for( cur = tail; cur; cur = cur->prev ) {
    if( cur->prev ) assert(cur->prev->next == cur);
    reverse_count++;
  }

  assert(forward_count == reverse_count);
  return forward_count;
}
#endif

static
void validate_cache(struct rdcache_s* tree)
{
#if VERIFY
  int top, bottom;
  struct top_entry_s* top_cur;
  struct cache_entry_s* bottom_cur;
  int count;
  int in_ain;
  int in_aout;
  int in_am;
  int num_used_pages = 0;
  int num_used_top_nodes = 0;
  int num_dirty = 0;

  // 0: All tree entries must be in either Ain, Aout, or Am,
  //    and num_entries is correct for each top entry.
  for(top = 0; top < TOP_SIZE; top++) {
    top_cur = tree->top_index_list[top];
    while (top_cur) {
      num_used_top_nodes++;
      count = 0;
      for( bottom = 0; bottom < BOTTOM_SIZE; bottom++ ) {
        bottom_cur = top_cur->bottom_index[bottom];
        while(bottom_cur) {
          // Check that it is in ain, aout, or am.
          in_ain = find_in_queue(tree->ain_head, bottom_cur);
          in_aout = find_in_queue(tree->aout_head, bottom_cur);
          in_am = find_in_queue(tree->am_lru_head, bottom_cur);
          assert( in_ain || in_aout || in_am );
          if( in_ain ) assert( bottom_cur->queue == QUEUE_AIN );
          if( in_aout ) assert( bottom_cur->queue == QUEUE_AOUT );
          if( in_am ) assert( bottom_cur->queue == QUEUE_AM );
          if( bottom_cur->page ) num_used_pages++;
          if( bottom_cur->dirty ) num_dirty++;
          bottom_cur = (struct cache_entry_s*)bottom_cur->base.next;
          count++;
        }
      }
      // Check that the count is correct.
      assert(count == top_cur->num_entries);
      top_cur = (struct top_entry_s*)top_cur->base.next;
    }
  }

  // 1: Entries in Ain must be in the tree
  in_ain = validate_queue(tree, tree->ain_head, tree->ain_tail, QUEUE_AIN);
  assert( in_ain == tree->ain_current );
  // 2: Entries in Aout must be in the tree
  in_aout = validate_queue(tree, tree->aout_head, tree->aout_tail, QUEUE_AOUT);
  assert( in_aout == tree->aout_current );
  // 3: Entries in Am must be in the tree
  in_am = validate_queue(tree, tree->am_lru_head, tree->am_lru_tail, QUEUE_AM);
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
    struct cache_entry_base_s* cur;
    int num_free_entries = 0;
    for( cur = tree->free_entries_head; cur; cur = cur->next ) {
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

  // 7: must not lose top nodes
  {
    struct cache_entry_base_s* cur;
    int num_free_top_nodes = 0;
    for( cur = tree->free_top_nodes_head; cur; cur = cur->next ) {
      num_free_top_nodes++;
    }
    assert( num_used_top_nodes + num_free_top_nodes == tree->max_top_nodes );
  }
#endif
}

static
void do_wait_for(struct rdcache_s* cache, cache_seqn_t sn)
{
  int index;
  cache_seqn_t at;
  cache_seqn_t max_completed = cache->completed_request_number;
  int last;

  DEBUG_PRINT(("wait_for(%i) completed=%i\n", (int) sn, (int) max_completed));

  // Do nothing if there are no pending entries.
  if( cache->pending_first_entry < 0 || cache->pending_last_entry < 0 ) return;

  // Do nothing if we don't have a valid sequence number to wait for.
  if( sn == NO_SEQUENCE_NUMBER ) return;

  // Do nothing if we have already completed sn.
  if( sn <= max_completed ) return;

  // Note: chpl_comm_wait_nb_some could cause a different task body to run...

  // If we have any pending requests with sequence number <= sn,
  // wait for them to complete.
  while( 1 ) {
    index = cache->pending_first_entry;
    if( index == -1 ) break;
    at = cache->pending_sequence_numbers[index];
    if( at <= sn ) {
      // Wait for some requests
      last = cache->pending_last_entry;
      if( last < index ) last = cache->pending_len - 1;
      while( cache->pending[index] ) {
        DEBUG_PRINT(("wait_for waiting %i..%i\n", index, last));
        // Wait for some requests to complete.
        chpl_comm_wait_nb_some(&cache->pending[index], last - index + 1);
      }
      // cache->pending[index] == NULL now
    }
    if( chpl_comm_test_nb_complete(cache->pending[index]) ) {
      // we completed cache->pending[index], so remove the entry from the queue.
      DEBUG_PRINT(("wait_for removing %i\n", index));
      fifo_circleb_pop( &cache->pending_first_entry, &cache->pending_last_entry, cache->pending_len);
      max_completed = seqn_max(max_completed, at);
    } else if( at > sn ) {
      // Stop if we have an uncompleted request for a later sequence number
      DEBUG_PRINT(("wait_for stopped at %i\n", index));
      break;
    }
  }

  cache->completed_request_number = max_completed;
}



// For the region of this page in raddr,len, we complete any pending/not
// started operations that possibly overlap with that region.
// If FLUSH_EVICT or FLUSH_INVALIDATE_PAGE is set, we will ignore the region.
static
void flush_entry(struct rdcache_s* cache, struct cache_entry_s* entry, int op,
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

          // Note: chpl_comm_put_nb could cause a different task body to run.
          handle =
            chpl_comm_put_nb(page+start, /*local addr*/
                             entry->base.node,
                             (void*)(entry->raddr+start),
                             got_len /*size*/,
                             CHPL_COMM_UNKNOWN_ID, -1, 0);

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
    free_page_list_entry = cache->free_page_list_entries_head;
    free_page_list_entry->page = page;
    SINGLE_POP_HEAD(cache, free_page_list_entries);
    SINGLE_PUSH_HEAD(cache, free_page_list_entry, free_pages);
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
void use_entry(struct rdcache_s* cache, struct cache_entry_s* entry)
{
  // If X is in Am then move it to the head of Am
  // If it's on the Am queue, move it to the front of the Am queue.
  if( entry->queue == QUEUE_AM ) {
    DOUBLE_REMOVE(cache, entry, am_lru);
    DOUBLE_PUSH_HEAD(cache, entry, am_lru);
  } 
  // Otherwise, leave it where it is.
  // Else If X is in A1in then do nothing
}

// Plumb a cache entry into the tree. We might need to replace something
// in Aout in the process, but we should not be calling this function
// to replace something in Ain or Am (ie anything with entry->page
// already set).
static
struct cache_entry_s* make_entry(struct rdcache_s* tree,
                                 c_nodeid_t node, raddr_t raddr,
                                 unsigned char* page)
{
  int top_idx, bottom_idx;
  uint32_t high_bits;
  uint32_t low_bits;
  struct top_entry_s **head, *top_match, *top_tmp;
  struct cache_entry_s **bottom, *bottom_match, *bottom_tmp;
  struct cache_entry_base_s* bottom_prev;

  assert(raddr != 0);

  top_idx = get_top_bits(raddr, node);
  bottom_idx = get_bottom_bits(raddr, node);
  high_bits = get_high_bits(raddr);
  low_bits = get_low_bits(raddr);

  head = &tree->top_index_list[top_idx];
  top_match = top_list_search(*head, high_bits, node, NULL);
  if (!top_match) {
    // create a new entry
    top_tmp = top_entry_allocate(tree);
    top_tmp->base.index_bits = high_bits;
    top_tmp->base.node = node;
    top_tmp->base.next = (struct cache_entry_base_s*)*head;
    top_tmp->num_entries = 0;
    memset(top_tmp->bottom_index, 0, BOTTOM_SIZE*sizeof(struct cache_entry_s*));

    *head = top_tmp;
    top_match = *head;
    DEBUG_PRINT(("  added a new top_index_entry: %p\n", top_match));
  }

  bottom = &top_match->bottom_index[bottom_idx];
  bottom_match = bottom_list_search(*bottom, low_bits, node, &bottom_prev);

  if( bottom_match ) {
  // If X is in A1out then find space for X and add it to the head of Am
    assert( bottom_match->base.node == node );
    assert( bottom_match->raddr == raddr );
    // We shouldn't be replacing something in Ain or Am; use use_entry instead
    assert(bottom_match->queue == QUEUE_AOUT);

    DEBUG_PRINT(("%d: Found %p in Aout\n", chpl_nodeID, (void*) raddr));
    // add X to the head of Am
    DOUBLE_REMOVE(tree, bottom_match, aout);
    tree->aout_current--;
    DOUBLE_PUSH_HEAD(tree, bottom_match, am_lru);
    tree->am_current++;

    bottom_match->queue = QUEUE_AM;
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

    // Fill in the entry...
    bottom_tmp->base.index_bits = low_bits;
    bottom_tmp->base.node = node;
    // Link to next hashtable element, adding not replacing.
    bottom_tmp->base.next = (struct cache_entry_base_s*) *bottom;

    bottom_tmp->raddr = raddr;
    bottom_tmp->queue = QUEUE_AIN;
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

    // Put it as the first element in the appropriate hashtable bucket.
    *bottom = bottom_tmp;
    bottom_match = bottom_tmp;
    top_match->num_entries++; // adding an element.
    DEBUG_PRINT(("  added a new bottom_index_entry: %p\n", bottom_match));
  }


  return bottom_match;
}

static
void cache_put(struct rdcache_s* cache,
                unsigned char* addr,
                c_nodeid_t node, raddr_t raddr, size_t size,
                cache_seqn_t last_acquire,
                int32_t commID, int ln, int32_t fn)
{
  struct cache_entry_s* entry;
  raddr_t ra_first_page;
  raddr_t ra_last_page;
  raddr_t ra_next_page;
  raddr_t ra_page_end;
  raddr_t ra_page;
  raddr_t requested_start, requested_end, requested_size;
  unsigned char* page;
  int entry_after_acquire;
  cache_seqn_t sn;

  DEBUG_PRINT(("cache_put %i:%p from %p len %i\n",
               (int) node, (void*) raddr, addr, (int) size));

  assert(chpl_nodeID != node); // should be handled in chpl_gen_comm_put

  // And don't do anything if it's a zero-length 
  if( size == 0 ) {
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

    // We will put from ra_page to ra_page_end
    ra_page_end = (ra_page==ra_last_page)?(ra_next_page):(ra_page+CACHEPAGE_SIZE);

    // Compute the portion of the page that was requested
    requested_start = raddr_max(raddr, ra_page);
    requested_end = raddr_min(raddr+size, ra_page_end);
    requested_size = requested_end - requested_start;

    // Is the page in the tree?
    entry = find_in_tree(cache, node, ra_page);
    page = NULL;

    // Ignore entries in Aout for now.
    if( entry && ! entry->page ) entry = NULL;

    if( entry ) {
      // Is this cache line available for use, based on when we
      // last ran an acquire fence?
      entry_after_acquire = ( entry->min_sequence_number >= last_acquire );
   
      // If the cache line contains any overlapping writes or prefetches,
      // we must wait for them to complete before we store new data.
      // Nonblocking GETs and PUTs must not have their buffers changed
      // during operation. And, we don't want an earlier prefetch to overwrite
      // our later write!
      flush_entry(cache, entry,
                  entry_after_acquire?FLUSH_PREPARE_PUT:FLUSH_INVALIDATE_PAGE,
                  requested_start, requested_size);
      page = entry->page;
    }

    if( ! page ) {
      // get a page from the free list.
      page = allocate_page(cache);
    }

    if( entry ) use_entry(cache, entry);
    else entry = make_entry(cache, node, ra_page, page);

    // Make sure we have a dirty structure.
    if( ! entry->dirty ) {
      allocate_dirty(cache, entry);
    } else {
      // Make it the most recently use dirty page.
      use_dirty(cache, entry->dirty);
    }

    // Now, set the dirty bits.
    set_valids_for_skip_len(entry->dirty->dirty,
                            requested_start & CACHEPAGE_MASK, requested_size,
                            CACHEPAGE_BITMASK_WORDS);

    // Op will be started for this in flush_entry for a dirty page.

    // This will increment next request number so cache events are recorded.
    sn = cache->next_request_number;
    cache->next_request_number++;
    // Set the minimum sequence number so an acquire fence before
    // the next read will cause this write to be disregarded.
    entry->min_sequence_number = seqn_min(entry->min_sequence_number, sn);

    // Copy the data into page.
    chpl_memcpy(page+(requested_start-ra_page),
                addr+(requested_start-raddr),
                requested_size);

    // Make sure that there is a dirty page available for next time.
    ensure_free_dirty(cache);
    // Make sure that there is an available page for next time.
    ensure_free_page(cache, NULL);
  }

  if( VERIFY ) validate_cache(cache);

#ifdef DUMP
  printf("After cache_put, cache is:\n");
  rdcache_print(cache);
#endif

}

/*
    // Handle prefetching. See http://www.ece.eng.wayne.edu/~sjiang/Tsinghua-2010/linux-readahead.pdf
    // for inspiration.
    // Here we decide whether or not we are going to prefetch the next page in the forward
    // or backward direction, and we also increase the request size if access looks sequential.
    trigger_prefetch = 0;
    nvalid_before = count_valid_lines_before(entry->valid_lines, ra_line >> CACHELINE_BITS);
    nvalid_after = count_valid_lines_at_after(entry->valid_lines, ra_line_end >> CACHELINE_BITS);
    if( nvalid_before >= PREFETCH_TRIGGER_LINES && nvalid_before > nvalid_after ) {
      trigger_prefetch = 1; // prefetch in the forward direction.
      // Ask for the rest of the page going forward.
      ra_line_end = ra_page + CACHEPAGE_SIZE;
    } else if( nvalid_after >= PREFETCH_TRIGGER_LINES && nvalid_after > nvalid_before ) {
      trigger_prefetch = -1; // prefetch in the reverse direction.
      // Ask for the rest of the page going backward.
      ra_line = ra_page;
    }

    TODO. use trigger_prefetch. Check page entry's trigger condition.

    // Should we increase what we're getting in order to do synchronous
    // prefetch, and should we trigger asynchronous prefetch?
    if( count_valid_lines_before(entry->valid_lines, 
*/

static inline
int is_congested(struct rdcache_s* cache)
{
  int have = fifo_circleb_count(cache->pending_first_entry,
                                cache->pending_last_entry,
                                cache->pending_len);
  return have > 3 * cache->pending_len / 2;
}

static
void cache_get(struct rdcache_s* cache,
                unsigned char * addr,
                c_nodeid_t node, raddr_t raddr, size_t size,
                cache_seqn_t last_acquire,
                int sequential_readahead_length,
                int32_t commID, int ln, int32_t fn);

static
void cache_get_trigger_readahead(struct rdcache_s* cache,
                                 c_nodeid_t node,
                                 raddr_t page_raddr,
                                 raddr_t request_raddr, size_t request_size,
                                 // skip < 0 -> reverse, >0 -> forward
                                 readahead_distance_t skip,
                                 readahead_distance_t len,
                                 cache_seqn_t last_acquire,
                                 int32_t commID, int ln, int32_t fn)
{
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
      INFO_PRINT(("%i starting readahead from %p to %p\n",
                  (int) chpl_nodeID, (void*) (prefetch_start), (void*) (prefetch_end)));
      cache_get(cache, NULL /* prefetch */,
                node,
                prefetch_start, prefetch_end - prefetch_start,
                last_acquire,
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
        CACHE_LINES_PER_PAGE_BITMASK_WORDS) > 1)
    return 1;
  if(count_valid_at_after(valid, skip+len,
        CACHE_LINES_PER_PAGE_BITMASK_WORDS) > 1)
    return -1;
  return 0;
}


// If addr == NULL, this will prefetch.
static
void cache_get(struct rdcache_s* cache,
                unsigned char * addr,
                c_nodeid_t node, raddr_t raddr, size_t size,
                cache_seqn_t last_acquire,
                int sequential_readahead_length,
                int32_t commID, int ln, int32_t fn)
{
  struct cache_entry_s* entry;
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
  int isprefetch = (addr == NULL);
  int entry_after_acquire;
  chpl_comm_nb_handle_t handle;
  uintptr_t readahead_len, readahead_skip;
  int ra;
#ifdef TIME
  struct timespec start_get1, start_get2, wait1, wait2;
#endif

  INFO_PRINT(("%i cache_get addr %p from %i:%p len %i ra_len %i\n",
               (int) chpl_nodeID, addr, (int) node, (void*) raddr, (int) size, sequential_readahead_length));

  assert(chpl_nodeID != node); // should be handled in chpl_gen_comm_prefetch.

  // And don't do anything if it's a zero-length 
  if( size == 0 ) {
    return;
  }

  // first_page = raddr of start of first needed page
  ra_first_page = round_down_to_mask(raddr, CACHEPAGE_MASK);
  // last_page = raddr of start of last needed page
  ra_last_page = round_down_to_mask(raddr+size-1, CACHEPAGE_MASK);
  // first_line = raddr of start of first needed line
  ra_first_line = round_down_to_mask(raddr, CACHELINE_MASK);
  // last_line = raddr of start of last needed line
  ra_last_line = round_down_to_mask(raddr+size-1, CACHELINE_MASK);
  ra_next_line = ra_last_line + CACHELINE_SIZE;

  // If the request is too large to reasonably fit in the cache, limit
  // the amount of data prefetched. (or do nothing?)
  if( isprefetch && (ra_last_page-ra_first_page)/CACHEPAGE_SIZE+1 > MAX_PAGES_PER_PREFETCH ) {
    ra_last_page = ra_first_page + CACHEPAGE_SIZE*MAX_PAGES_PER_PREFETCH;
  }

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
    entry = find_in_tree(cache, node, ra_page);
    page = NULL;


    // Ignore entries in Aout for now.
    if( entry && ! entry->page ) entry = NULL;

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

    // If we are doing a GET that is adjacent to the last GET,
    // fetch the rest of the data in this page.
    readahead_skip = 0;
    readahead_len = 0;
    if( ENABLE_READAHEAD &&
        entry_after_acquire &&
        sequential_readahead_length == 0 &&
        ! has_data &&
        ! (entry && entry->readahead_len) ) {
      
      ra = 0;

      if( ENABLE_READAHEAD_TRIGGER_WITHIN_PAGE && entry ) {
        ra = should_readahead_extend(entry->valid_lines,
                                 (ra_line - ra_page) >> CACHELINE_BITS,
                                 (ra_line_end - ra_line) >> CACHELINE_BITS);
        if( ra ) {
          INFO_PRINT(("%i readahead trigger within-page direction=%i from %p to %p last %p\n",
               (int) chpl_nodeID, ra, (void*) ra_line, (void*) ra_line_end, (void*) cache->last_cache_miss_read_addr));
        }
      }
     
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
    }

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
        if( !isprefetch ) {
          if( entry->max_prefetch_sequence_number > cache->completed_request_number ) {
#ifdef TIME
            clock_gettime(CLOCK_REALTIME, &wait1);
#endif

            wait_for(cache, entry->max_prefetch_sequence_number);

#ifdef TIME
            clock_gettime(CLOCK_REALTIME, &wait2);

            printf("%li ns waiting for %p\n", time_duration(&wait1, &wait2), (void*) ra_page);
#endif
          }
        }
        // If the cache line is in Am, move it to the front of Am.
        use_entry(cache, entry);
        if( ! isprefetch ) {
      
          //printf("cache hit on page %i:%p %p ra_len %i\n", 
          //       node, (void*) ra_page, (void*) requested_start,
          //       (int) entry->readahead_len);
          // Copy the data out.
          chpl_memcpy(addr+(requested_start-raddr),
                      page+(requested_start-ra_page),
                      requested_size);
    
          // If we are accessing a page that has a readahead condition,
          // trigger that readahead.
          if( ENABLE_READAHEAD && entry->readahead_len ) {
            // Remove the readahead trigger from this page since
            // we're starting the readahead now.
            readahead_skip = entry->readahead_skip;
            readahead_len = entry->readahead_len;
            entry->readahead_skip = 0;
            entry->readahead_len = 0;

            cache_get_trigger_readahead(cache, node, ra_page,
                                        raddr, size,
                                        readahead_skip,
                                        readahead_len,
                                        last_acquire,
                                        commID, ln, fn);
            entry = NULL; // note trigger readahead could evict entry...
          }
        }

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
      flush_entry(cache, entry,
                  entry_after_acquire?FLUSH_PREPARE_GET:FLUSH_INVALIDATE_PAGE,
                  ra_line, ra_line_end-ra_line);
    }

    // Otherwise -- start a get !

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

#ifdef TIME
    clock_gettime(CLOCK_REALTIME, &start_get1);
#endif
    // Note: chpl_comm_get_nb could cause a different task body to run.
    handle = 
      chpl_comm_get_nb(page+(ra_line-ra_page), /*local addr*/
                       node, (void*) ra_line,
                       ra_line_end - ra_line /*size*/,
                       commID, ln, fn);
#ifdef TIME
    clock_gettime(CLOCK_REALTIME, &start_get2);
#endif

    // Now, while that get is going, plumb into the tree.

    if( entry ) {
      use_entry(cache, entry);
    } else {
      entry = make_entry(cache, node, ra_page, page);
    }

    // Set the valid lines
    set_valid_lines(entry->valid_lines,
                    (ra_line - ra_page) >> CACHELINE_BITS,
                    (ra_line_end - ra_line) >> CACHELINE_BITS);

    if( ! isprefetch ) {
      // This will increment next request number so cache events are recorded.
      sn = cache->next_request_number;
      cache->next_request_number++;
    } else {
      // For a prefetch, store sequence number and record operation handle.

      // This will increment next request number so cache events are recorded.
      sn = pending_push(cache, handle);
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
    // This could happen if entry is the last element of Ain..
    ensure_free_page(cache, entry);

    if( ! isprefetch ) {
      // If we're not prefetching... wait for the get to complete and copy it
      // back out of the cache.

#ifdef TIME
      clock_gettime(CLOCK_REALTIME, &wait1);
#endif

      chpl_comm_wait_nb_some(&handle, 1);

#ifdef TIME
      clock_gettime(CLOCK_REALTIME, &wait2);

      printf("after get %p\n%li ns in start_get\n%li ns in wait_some\n%li ns total\n",
             (void*) ra_line,
             time_duration(&start_get1, &start_get2),
             time_duration(&wait1, &wait2),
             time_duration(&start_get1, &wait2));
#endif

      // Then, copy it out.
      chpl_memcpy(addr+(requested_start-raddr),
                  page+(requested_start-ra_page),
                  requested_size);
  
    }
  }

  if( VERIFY ) validate_cache(cache);

#ifdef DUMP
  printf("After cache_get cache is:\n");
  rdcache_print(cache);
#endif
}


static
void cache_invalidate(struct rdcache_s* cache,
                       c_nodeid_t node, raddr_t raddr, size_t size)
{
  struct cache_entry_s* entry;
  raddr_t ra_first_page;
  raddr_t ra_last_page;
  raddr_t ra_next_page;
  raddr_t ra_page;
  raddr_t ra_page_end;
  raddr_t requested_start, requested_end, requested_size;

  DEBUG_PRINT(("shared_invalidate from %i:%p len %i\n",
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

    // Is the page in the tree?
    entry = find_in_tree(cache, node, ra_page);

    if( entry && entry->page ) {
      flush_entry(cache, entry, FLUSH_INVALIDATE_REGION, requested_start, requested_size);
    }
  }
}

static
void cache_clean_dirty(struct rdcache_s* cache)
{
  struct dirty_entry_s* cur;

  while( 1 ) {
    cur = cache->dirty_lru_head;
    // Dirty records with page entries are before free ones without
    if( ! cur || ! cur->entry ) break;
    flush_entry(cache, cur->entry, FLUSH_DO_CLEAR_DIRTY, 0, CACHEPAGE_SIZE);
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
  chpl_task_prvData_t* task_local = chpl_task_getPrvData();
  return &task_local->comm_data.cache_data;
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
  
    // Quick configuration check...
    assert(OTHER_BITS+TOP_BITS+OTHER_BITS+BOTTOM_BITS+CACHEPAGE_BITS == 64);
    assert(HALF_BITS + HALF_BITS + CACHEPAGE_BITS == 64);

    // Otherwise, we will need some thread-local storage.
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

    TRACE_PRINT(("%d: task %d in chpl_cache_fence(acquire=%i,release=%i) on cache %p from %s:%d\n", chpl_nodeID, (int) chpl_task_getId(), acquire, release, cache, fn?fn:"", ln));
    //printf("%d: task %d in chpl_cache_fence(acquire=%i,release=%i) on cache %p from %s:%d\n", chpl_nodeID, (int) chpl_task_getId(), acquire, release, cache, fn?fn:"", ln);

#ifdef DUMP
    DEBUG_PRINT(("%d: task %d before fence\n", chpl_nodeID, (int) chpl_task_getId()));
    chpl_cache_print();
#endif

    if( acquire ) {
      task_local->last_acquire = cache->next_request_number;
      cache->next_request_number++;
    }

    if( release ) {
      cache_clean_dirty(cache);
      wait_all(cache);
    }
#ifdef DUMP
    DEBUG_PRINT(("%d: task %d after fence\n", chpl_nodeID, (int) chpl_task_getId()));
    chpl_cache_print();
#endif
  }
  // Do nothing if cache is not enabled.
}

// If a transfer is large enough we should directly initiate it to avoid
// overheads of going through the cache
static inline
int size_merits_direct_comm(struct rdcache_s* cache, size_t size)
{
  return size >= CACHEPAGE_SIZE;
}

void chpl_cache_comm_put(void* addr, c_nodeid_t node, void* raddr,
                         size_t size, int32_t commID, int ln, int32_t fn)
{
  //printf("put len %d node %d raddr %p\n", (int) len * elemSize, node, raddr);
  struct rdcache_s* cache = tls_cache_remote_data();
  if (size_merits_direct_comm(cache, size)) {
    cache_invalidate(cache, node, (raddr_t)raddr, size);
    chpl_comm_put(addr, node, raddr, size, commID, ln, fn);
    return;
  }
  chpl_cache_taskPrvData_t* task_local = task_private_cache_data();
  TRACE_PRINT(("%d: task %d in chpl_cache_comm_put %s:%d put %d bytes to %d:%p "
               "from %p\n",
               chpl_nodeID, (int)chpl_task_getId(), chpl_lookupFilename(fn), ln,
               (int)size, node, raddr, addr));
  chpl_comm_diags_verbose_rdma("get", node, size, ln, fn, commID);

#ifdef DUMP
  chpl_cache_print();
#endif

  //saturating_increment(&info->put_since_release);
  //task_local->last_op = seqn_max(cache, addr, node, raddr, size);
  cache_put(cache, addr, node, (raddr_t)raddr, size, task_local->last_acquire,
            commID, ln, fn);
  return;
}

void chpl_cache_comm_get(void *addr, c_nodeid_t node, void* raddr,
                         size_t size, int32_t commID, int ln, int32_t fn)
{
  //printf("get len %d node %d raddr %p\n", (int) len * elemSize, node, raddr);
  struct rdcache_s* cache = tls_cache_remote_data();
  if (size_merits_direct_comm(cache, size)) {
    cache_invalidate(cache, node, (raddr_t)raddr, size);
    chpl_comm_get(addr, node, raddr, size, commID, ln, fn);
    return;
  }
  chpl_cache_taskPrvData_t* task_local = task_private_cache_data();
  TRACE_PRINT(("%d: task %d in chpl_cache_comm_get %s:%d get %d bytes from "
               "%d:%p to %p\n",
               chpl_nodeID, (int)chpl_task_getId(), chpl_lookupFilename(fn), ln,
               (int)size, node, raddr, addr));
  chpl_comm_diags_verbose_rdma("put", node, size, ln, fn, commID);

#ifdef DUMP
  chpl_cache_print();
#endif

  //saturating_increment(&info->get_since_acquire);
  cache_get(cache, addr, node, (raddr_t)raddr, size, task_local->last_acquire,
            0, commID, ln, fn);

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
  //saturating_increment(&info->prefetch_since_acquire);
  cache_get(cache, NULL, node, (raddr_t)raddr, size, task_local->last_acquire,
            0, CHPL_COMM_UNKNOWN_ID, ln, fn);
}
void chpl_cache_comm_get_strd(void *addr, void *dststr, c_nodeid_t node,
                              void *raddr, void *srcstr, void *count,
                              int32_t strlevels, size_t elemSize,
                              int32_t commID, int ln, int32_t fn) {
  TRACE_PRINT(("%d: in chpl_cache_comm_get_strd\n", chpl_nodeID));
  // do a full fence - so that:
  // 1) any pending writes are completed (in case they were to the
  //    same location handled by the strided get)
  // 2) the cache does not have older values than what we're getting now
  // Alternatively, we could invalidate the requested regions.
  // Alternatively, the strided get could be done through the cache
  // system. This is just the current (possibly temporary) solution.
  chpl_cache_fence(1, 1, ln, fn);
  // do the strided get.
  chpl_comm_get_strd(addr, dststr, node, raddr, srcstr, count, strlevels,
                     elemSize, commID, ln, fn);
}
void chpl_cache_comm_put_strd(void *addr, void *dststr, c_nodeid_t node,
                              void *raddr, void *srcstr, void *count,
                              int32_t strlevels, size_t elemSize,
                              int32_t commID, int ln, int32_t fn) {
  TRACE_PRINT(("%d: in chpl_cache_comm_put_strd\n", chpl_nodeID));
  // do a full fence - so that:
  // 1) any pending writes are completed (in case they were to the
  //    same location handled by the strided put and would
  //    complete in the wrong order)
  // 2) the cache does not keep older values from before the put.
  // Alternatively, the strided put could be done through the cache
  // system. This is just the current (possibly temporary) solution.
  chpl_cache_fence(1, 1, ln, fn);
  // do the strided put.
  chpl_comm_put_strd(addr, dststr, node, raddr, srcstr, count, strlevels,
                     elemSize, commID, ln, fn);
}

//
// Directly initiate unordered comm, invalidate any pending updates to
// overlapping regions beforehand.
//
void chpl_cache_comm_put_unordered(void* addr, c_nodeid_t node, void* raddr,
                                   size_t size, int32_t commID, int ln, int32_t fn)
{
  struct rdcache_s* cache = tls_cache_remote_data();
  cache_invalidate(cache, node, (raddr_t)raddr, size);
  chpl_comm_put_unordered(addr, node, raddr, size, commID, ln, fn);

}

void chpl_cache_comm_get_unordered(void *addr, c_nodeid_t node, void* raddr,
                                   size_t size, int32_t commID, int ln, int32_t fn)
{
  struct rdcache_s* cache = tls_cache_remote_data();
  cache_invalidate(cache, node, (raddr_t)raddr, size);
  chpl_comm_get_unordered(addr, node, raddr, size, commID, ln, fn);
}


void chpl_cache_comm_getput_unordered(c_nodeid_t dstnode, void* dstaddr,
                                      c_nodeid_t srcnode, void* srcaddr,
                                      size_t size, int32_t commID,
                                      int ln, int32_t fn)
{
    struct rdcache_s* cache = tls_cache_remote_data();
    cache_invalidate(cache, srcnode, (raddr_t)srcaddr, size);
    cache_invalidate(cache, dstnode, (raddr_t)dstaddr, size);
    chpl_comm_getput_unordered(dstnode, dstaddr, srcnode, srcaddr, size, commID, ln, fn);
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

/*
// Turn the cache on or off for debug purposes.
void chpl_cache_set_enabled(int enabled)
{
  chpl_cache_fence(1,1, -1, NULL);
  if( CHPL_CACHE_REMOTE == 0 && enabled == 1 ) chpl_cache_do_init();
  CHPL_CACHE_REMOTE = enabled;
}
*/

#endif
// end ifdef HAS_CHPL_CACHE_FNS

