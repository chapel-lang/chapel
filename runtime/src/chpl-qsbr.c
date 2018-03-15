/*
 * Copyright 2004-2018 Cray Inc.
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

#include "chplrt.h"
#include "chpl-thread-local-storage.h"
#include "chpl-mem.h"
#include "chpl-mem-desc.h"

#include "chpl-tasks.h"
#include "chpl-qsbr.h"

// Profile counters...
#ifndef CHPL_QSBR_PROFILE
#define CHPL_QSBR_PROFILE 0
#endif

// Correctness checks...
#ifndef CHPL_QSBR_CHECK
#define CHPL_QSBR_CHECK 0
#endif

#if CHPL_QSBR_PROFILE
static atomic_uint_least64_t nParked = 0;
static atomic_uint_least64_t nUnparked = 0;
static atomic_uint_least64_t nCheckpoints = 0;
#endif

// We maintain a single global counter that denotes the current epoch.
// Each time some deletion is published (deferred), we advance the epoch
// by one. At each checkpoint, a thread will update their thread-local epoch
// to the global one. Memory that is deferred for deletion is only safe to
// reclaim after all thread-local epochs exceed the epoch of when the memory was
// marked for deletion.
static atomic_uint_least64_t global_epoch = 0;

// We allow users to disable memory reclamation at runtime to allow 
// for unregistered threads to access protected data, which is useful
// if the user calls out to C code (like OpenMP) which may call back
// into Chapel code, potentially accessing the protected data without
// registering. If memory reclamation is disabled, we still allow the
// reclamation of all memory that was deferred for deletion prior to
// being disabled; this is safe as all memory must be logically deleted
// and therefore not accessible to future tasks and threads prior to 
// being deferred for deletion. This is used when deciding the safest
// epoch, being the minimum.
#define CHPL_QSBR_ENABLED_SAFE_EPOCH ((uint64_t) -1)
static atomic_uint_least64_t disabled_safe_epoch = CHPL_QSBR_ENABLED_SAFE_EPOCH;

// Each node in the deferred list has associated with it
// the data (and numData, useful for determining if it
// is an array and should be treated as a 'void **') and
// the targetEpoch which is the epoch required for all threads
// to have observed before the data can be deleted. By making
// use of the fact that (barring integer overflow) the largest
// targetEpoch is always pushed as the head, we have a relative
// sorting order. If we ask for some targetEpoch X, and for some
// node N we have X >= N.targetEpoch, then we can claim N and
// N.next and so and so forth since N.targetEpoch >= N.next.targetEpoch. 
struct defer_node {
  uint64_t targetEpoch;
  void *data;
  int numData;
  struct defer_node *next;
};



// List of thread-local data.
struct tls_node *chpl_qsbr_tls_list;
CHPL_TLS_DECL(struct tls_node *, chpl_qsbr_tls);
int chpl_qsbr_is_enabled = -1;
pthread_once_t chpl_qsbr_once_flag = PTHREAD_ONCE_INIT;

// As multiple threads can register themselves at a later time using atomic 
// RMW operations, all loads of the list must also be atomic.
static inline struct tls_node *get_tls_list(void);
static inline struct tls_node *get_tls_list(void) {
  #if CHPL_QSBR_CHECK
  if (chpl_qsbr_tls_list == NULL) {
    chpl_internal_error("'chpl_qsbr_tls_list' is NULL!\n");
  }
  #endif
  return (struct tls_node *) atomic_load_uintptr_t((uintptr_t *) &chpl_qsbr_tls_list);
}

static inline uint64_t get_epoch(struct tls_node *node);
static inline uint64_t get_epoch(struct tls_node *node) {
  return atomic_load_uint_least64_t(&node->epoch);
}

static inline void set_epoch(struct tls_node *node, uint64_t epoch);
static inline void set_epoch(struct tls_node *node, uint64_t epoch) {
  return atomic_store_uint_least64_t(&node->epoch, epoch);
}

static inline uint64_t get_global_epoch(void);
static inline uint64_t get_global_epoch(void) {
  return atomic_load_uint_least64_t(&global_epoch);
}

static inline uint64_t advance_global_epoch(void);
static inline uint64_t advance_global_epoch(void) {
  return atomic_fetch_add_uint_least64_t(&global_epoch, 1) + 1;
}

static inline void observe_epoch(struct tls_node *node);
static inline void observe_epoch(struct tls_node *node) {
  atomic_store_uint_least64_t(&node->epoch, get_global_epoch());
}

static inline uint64_t get_disabled_epoch(void);
static inline uint64_t get_disabled_epoch(void) {
  return atomic_load_uint_least64_t(&disabled_safe_epoch);
}

static inline void reset_disabled_epoch(void);
static inline void reset_disabled_epoch(void) {
  atomic_store_uint_least64_t(&disabled_safe_epoch, CHPL_QSBR_ENABLED_SAFE_EPOCH);
}

static inline void set_disabled_epoch(uint64_t epoch);
static inline void set_disabled_epoch(uint64_t epoch) {
  atomic_store_uint_least64_t(&disabled_safe_epoch, epoch);
}

static inline uint64_t is_parked(struct tls_node *node);
static inline uint64_t is_parked(struct tls_node *node) {
  return atomic_load_uint_least64_t(&node->parked);
}

static inline void park(struct tls_node *node);
static inline void park(struct tls_node *node) {
  #if CHPL_QSBR_CHECK
  if (is_parked(node)) {
    chpl_warning("Called park when already parked...\n", 0, 0);
  }
  #endif

  atomic_store_uint_least64_t(&node->parked, 1);
  
  #if CHPL_QSBR_PROFILE
  atomic_fetch_add_uint_least64_t(&nParked, 1);
  #endif
}

static inline void unpark(struct tls_node *node);
static inline void unpark(struct tls_node *node) {
  #if CHPL_QSBR_CHECK
  if (!is_parked(node)) {
    chpl_warning("Called unpark when not parked...\n", 0, 0);
  }
  #endif

  atomic_store_uint_least64_t(&node->parked, 0);
  
  #if CHPL_QSBR_PROFILE
  atomic_fetch_add_uint_least64_t(&nUnparked, 1);
  #endif
}

static inline void acquire_spinlock(struct tls_node *node, uintptr_t value);
static inline void acquire_spinlock(struct tls_node *node, uintptr_t value) {
  // Fast path: Quick acquire spinlock
  if (!atomic_compare_exchange_strong_uint_least64_t(&node->spinlock, 0, value)) {
    // Slow path: TestAndTestAndSet. Note that we cannot yield or
    // we will end up calling the checkpoint.
    while (true) {
      uint64_t spinlock = atomic_load_uint_least64_t(&node->spinlock);
      
      #if CHPL_QSBR_CHECK
      // Occurs if current task gets preempted while we hold spinlock
      // or if we attempted to recurse on a spinlock/forgot to release 
      if (spinlock == value) {
        chpl_internal_error("Attempt to acquire spinlock when already acquired...\n");
      }
      #endif

      if (spinlock == 0 
        && atomic_compare_exchange_weak_uint_least64_t(&node->spinlock, 0, value)) {
        break;
      }
    }
  }
}

static inline void release_spinlock(struct tls_node *node);
static inline void release_spinlock(struct tls_node *node) {
  #if CHPL_QSBR_CHECK
  if (atomic_load_uint_least64_t(&node->spinlock) != (uintptr_t) node) {
    chpl_internal_error("Attempt to release spinlock when not held by us...");
  }
  #endif

  atomic_store_uint_least64_t(&node->spinlock, 0);
}

// Requires spinlock.
/*static inline struct defer_node *get_defer_list(struct tls_node *node);
static inline struct defer_node *get_defer_list(struct tls_node *node) {
  return node->deferList;
}*/

// Requires spinlock. Pops all items less than or equal to 'epoch'.
static inline struct defer_node *pop_bulk_defer_list(struct tls_node *node, uint64_t epoch);
static inline struct defer_node *pop_bulk_defer_list(struct tls_node *node, uint64_t epoch) {
  #if CHPL_QSBR_CHECK
  if (atomic_load_uint_least64_t(&node->spinlock) != (uintptr_t) node) {
    chpl_internal_error("Attempt to mutate data without holding spinlock...");
  }
  #endif

  struct defer_node *dnode = node->deferList, *prev = NULL;
  while (dnode != NULL) {
    if (epoch >= dnode->targetEpoch) {
      break;
    }

    prev = dnode;
    dnode = dnode->next;
  }

  // If both nodes are NULL, then the list was actually empty
  if (dnode == NULL && prev == NULL) {
    return NULL;
  }

  // If prev is NULL then we have the actual head of the list
  if (prev == NULL) {
    node->deferList = NULL;
    return dnode;
  }

  // If neither are NULL, we need to correct list so previous points to NULL
  prev->next = NULL;
  return dnode;
}

// Requires spinlock.
static inline struct defer_node *pop_all_defer_list(struct tls_node *node);
static inline struct defer_node *pop_all_defer_list(struct tls_node *node) {
  #if CHPL_QSBR_CHECK
  if (atomic_load_uint_least64_t(&node->spinlock) != (uintptr_t) node) {
    chpl_internal_error("Attempt to mutate data without holding spinlock...");
  }
  #endif

  struct defer_node *head = node->deferList;
  node->deferList = NULL;
  return head;
}

// Requires spinlock.
static inline void push_defer_list(struct tls_node *node, struct defer_node *dnode);
static inline void push_defer_list(struct tls_node *node, struct defer_node *dnode) {
  #if CHPL_QSBR_CHECK
  if (atomic_load_uint_least64_t(&node->spinlock) != (uintptr_t) node) {
    chpl_internal_error("Attempt to mutate data without holding spinlock...");
  }
  #endif

  dnode->next = node->deferList;
  node->deferList = dnode;
}

// Must be called from the thread owning the node.
static inline struct defer_node *pop_recycle_list(struct tls_node *node);
static inline struct defer_node *pop_recycle_list(struct tls_node *node) {
  struct defer_node *dnode = node->recycleList;
  
  // If empty, return fresh allocation
  if (dnode == NULL) {
    return chpl_mem_calloc(1, sizeof(struct defer_node), CHPL_RT_MD_QSBR, 0, 0);
  }

  node->recycleList = dnode->next;

  // Sanitize before return...
  memset(dnode, 0, sizeof(struct defer_node));
  return dnode;
}

// Must be called from the thread owning the node.
static inline void push_recycle_list(struct tls_node *node, struct defer_node *dnode);
static inline void push_recycle_list(struct tls_node *node, struct defer_node *dnode) {
  dnode->next = node->recycleList;
  node->recycleList = dnode;
}

// Deletes data held by 'dnode', does not free 'dnode'
static inline void delete_data(struct defer_node *dnode);
static inline void delete_data(struct defer_node *dnode) {
  if (dnode->numData) {
      void **arr = dnode->data;
      for (int i = 0; i < dnode->numData; i++) {
          chpl_mem_free(arr[i], 0, 0);
      }
  }
  chpl_mem_free(dnode->data, 0, 0);
}

// Initializes TLS; should only need to be called once.
void chpl_qsbr_init_tls(void);
void chpl_qsbr_init_tls(void) {
  struct tls_node *node = chpl_mem_calloc(1, sizeof(struct tls_node), CHPL_RT_MD_QSBR, 0, 0);
  node->epoch = get_global_epoch();

  // Append to head of list
  struct tls_node *old_head;
  do {
    old_head = chpl_qsbr_tls_list;
    node->next = old_head;
  } while (!atomic_compare_exchange_weak_uintptr_t(
      (uintptr_t *) &chpl_qsbr_tls_list, (uintptr_t) old_head, (uintptr_t) node)
    );

  CHPL_TLS_SET(chpl_qsbr_tls, node);
}

// Obtains the minimum epoch of all threads and of the disabled safest epoch.
static uint64_t safe_epoch(void);
static uint64_t safe_epoch(void) {
  uint64_t min = get_disabled_epoch();
  // Check all remaining threads for whether they have updated to a more recent epoch.
  for (struct tls_node *node = get_tls_list(); node != NULL; node = node->next) {
    if (!is_parked(node)) {
      uint64_t epoch = get_epoch(node);
      min = (min < epoch) ? min : epoch;       
    }
  } 
  
  return min;
}

// Requires spinlock.
static inline void handle_deferred_data(struct tls_node *node);
static inline void handle_deferred_data(struct tls_node *node) {
  #if CHPL_QSBR_CHECK
  if (atomic_load_uint_least64_t(&node->spinlock) != (uintptr_t) node) {
    chpl_internal_error("Attempt to mutate data without holding spinlock...");
  }
  #endif

  // Acquire all data that can be deleted.
  uint64_t epoch = safe_epoch();
  struct defer_node *list = pop_bulk_defer_list(node, epoch);

  // Handle deletion of said data...
  // WARNING: Due to how `--memTrack` works, it is possible for
  // it to trigger a task-switch when it acquires the memTrack_sync
  // auxiliary sync var so we must handle this outside of the spinlock.
  release_spinlock(node);
  for (struct defer_node *dnode = list; dnode != NULL;) {
    struct defer_node *tmp = dnode;
    dnode = dnode->next;
    delete_data(tmp);
    push_recycle_list(node, tmp);
  }
  acquire_spinlock(node, (uintptr_t) node);
}

// Requires spinlock for both tls_nodes. Transfers deferred deletion from giver to receiver
static inline void give_deferred(struct tls_node *giver, struct tls_node *receiver);
static inline void give_deferred(struct tls_node *giver, struct tls_node *receiver) {
  #if CHPL_QSBR_CHECK
  if (atomic_load_uint_least64_t(&giver->spinlock) != (uintptr_t) giver
    || atomic_load_uint_least64_t(&receiver->spinlock) != (uintptr_t) giver) {
    chpl_internal_error("Attempt to transfer data without holding both spinlocks...");
  }
  #endif
  struct defer_node *list = pop_all_defer_list(giver);
  
  if (receiver->deferList == NULL) {
    receiver->deferList = list;
  } else {
    // Merge both lists in order of decreasing targetEpoch
    struct defer_node *newHead = NULL;
    struct defer_node *newTail = NULL;
    while (list != NULL && receiver->deferList != NULL) {
      struct defer_node *dnode;
      if (list->targetEpoch >= receiver->deferList->targetEpoch) {
        dnode = list;
        list = list->next;
      } else {
        dnode = receiver->deferList;
        receiver->deferList = receiver->deferList->next;
      }

      if (newHead == NULL) {
        newHead = newTail = dnode;
        dnode->next = NULL;
      } else {
        newTail->next = dnode;
        newTail = dnode;
        dnode->next = NULL;
      }
    }
    // Append extra
    if (list != NULL) {
      newTail->next = list;
      list = NULL;
    } else if (receiver->deferList != NULL) {
      newTail->next = receiver->deferList;
      receiver->deferList = NULL;
    }

    #if CHPL_QSBR_CHECK
    {
      struct defer_node *prev = NULL;
      struct defer_node *curr = newHead;

      while (curr != NULL) {
        if (prev != NULL && prev->targetEpoch > curr->targetEpoch) {
          chpl_internal_error("Merged list is not in sorted order!");
        }
        prev = curr;
        curr = curr->next;
      }
    }
    #endif
    receiver->deferList = newHead;
  }
}

void chpl_qsbr_init(void) {
    CHPL_TLS_INIT(chpl_qsbr_tls);
}

void chpl_qsbr_unblocked(void) {
  struct tls_node *tls = CHPL_TLS_GET(chpl_qsbr_tls);
  assert(tls != NULL);

  // Notify that we are now in-use.
  unpark(tls);
}


void chpl_qsbr_blocked(void) {
  struct tls_node *tls = CHPL_TLS_GET(chpl_qsbr_tls);

  // Initialize TLS...
  if (tls == NULL) {
    chpl_qsbr_init_tls();
    tls = CHPL_TLS_GET(chpl_qsbr_tls);
  }

  acquire_spinlock(tls, (uintptr_t) tls);
  park(tls);

  // Empty as best we can
  if(tls->deferList != NULL) {
    handle_deferred_data(tls);
  }

  // Check if fully emptied.
  if (tls->deferList == NULL) {
    release_spinlock(tls);
    return;
  }

  // At this point if we have any deferred data, we must pass this to some
  // other thread, as we could be parked indefinitely. Find first active
  // thread to dump the rest of our work on...
  struct tls_node *node = get_tls_list();
  for (; node != NULL; node = node->next) {
    if (node != tls && !is_parked(node)) {
      // Double check...
      acquire_spinlock(node, (uintptr_t) tls);
      if (!is_parked(node)) {
        give_deferred(tls, node);
        release_spinlock(node);
        release_spinlock(tls);
        return;
      }
      release_spinlock(node);  
    }
  }
  // all threads are finished so we should be able to safely handle our deferred data...
  handle_deferred_data(tls);
  release_spinlock(tls);
}

void chpl_qsbr_checkpoint(void) {
    #if CHPL_QSBR_PROFILE
    atomic_fetch_add_uint_least64_t(&nCheckpoints, 1);
    #endif
    struct tls_node *tls = CHPL_TLS_GET(chpl_qsbr_tls);
    
    // Initialize TLS...
    if (tls == NULL) {
      chpl_qsbr_init_tls();
      tls = CHPL_TLS_GET(chpl_qsbr_tls);
    }

    // Observe the current epoch.
    observe_epoch(tls);

    // Check if we have passed enough checkpoints to process our deferred data.
    if (tls->deferList != NULL) {
      acquire_spinlock(tls, (uintptr_t) tls);
      handle_deferred_data(tls);
      release_spinlock(tls);
    } 
}

static void _defer_deletion(void *data, int numData) {
  struct tls_node *tls = CHPL_TLS_GET(chpl_qsbr_tls);

  // Setup TLS if needed
  if (tls == NULL) {
    chpl_qsbr_init_tls();
    tls = CHPL_TLS_GET(chpl_qsbr_tls);
  }

  // Broadcast our state change
  uint64_t epoch = advance_global_epoch();
  set_epoch(tls, epoch);

  // Defer deletion of the data until all threads are
  // at least at the received epoch.
  struct defer_node *dnode = pop_recycle_list(tls);
  dnode->targetEpoch = epoch;
  dnode->data = data;
  dnode->numData = numData;

  // Defer deletion of our node.
  acquire_spinlock(tls, (uintptr_t) tls);
  push_defer_list(tls, dnode);
  release_spinlock(tls);
}

void chpl_qsbr_defer_deletion(void *data) {
    _defer_deletion(data, 0);
}

void chpl_qsbr_defer_deletion_multi(void **arrData, int numData) {
    _defer_deletion(arrData, numData);
}

void chpl_qsbr_disable(void) {
  uint64_t minEpoch = safe_epoch();

  // If no thread is registered it will return the 'ENABLED_SAFE_EPOCH' constant...
  if (minEpoch == CHPL_QSBR_ENABLED_SAFE_EPOCH) {
    minEpoch = 0;
  }

  set_disabled_epoch(minEpoch);
}

void chpl_qsbr_enable(void) {
  reset_disabled_epoch();
}


void chpl_qsbr_exit(void) {
    #if CHPL_QSBR_PROFILE
    printf("nParks: %lu, nUnparks: %lu, nCheckpoints: %lu\n", 
      atomic_load_uint_least64_t(&nParked), atomic_load_uint_least64_t(&nUnparked), atomic_load_uint_least64_t(&nCheckpoints));
    #endif

    // Clean thread-local storage
    while (chpl_qsbr_tls_list) {
        struct tls_node *node = chpl_qsbr_tls_list;
        chpl_qsbr_tls_list = chpl_qsbr_tls_list->next;

        while (node->deferList) {
            struct defer_node *dnode = node->deferList;
            node->deferList = node->deferList->next;

            delete_data(dnode);
            chpl_mem_free(dnode, 0, 0);
        }

        while (node->recycleList) {
            struct defer_node *dnode = node->recycleList;
            node->recycleList = node->recycleList->next;
            chpl_mem_free(dnode, 0, 0);
        }

        chpl_mem_free(node, 0, 0);
    }
}
