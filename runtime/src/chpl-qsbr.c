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
#include "chpl-atomics.h"
#include "chpl-tasks.h"
#include "chpl-qsbr.h"

// Number of checkpoints to pass before processing
#define CHPL_QSBR_CHECKPOINTS_PER_PROCESS 8



static atomic_uint_least64_t global_epoch = 0;

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

// Thread-local data that keeps track of our observed global state.
struct tls_node {
  /*
    The number of checkpoints passed through
  */
  uint64_t passedCheckpoints;
  /*
    The current observed epoch. This is used to ensure that writers
    do not delete an older instance that corresponds to this epoch.
  */
  atomic_uint_least64_t epoch;
  /*
    Number of tasks that this thread has running. Required so
    that threads without tasks (and no longer are able to call
    checkpoints) are not taken into account.
  */
  atomic_uint_least64_t nTasks;
  /*
    List of objects that are queued for deferred deletion, sorted by target_epoch. 
    We defer deletion if we find that while scanning the list of all thread-specific
    data, they are not up-to-date on that epoch. By sorting by target_epoch, we can
    make shortcuts by removing a large bulk of data at once. The list is protected
    by a lightweight spinlock, but is acquired using pseudo-TestAndSet operations
    due to low contention.
  */
  atomic_uint_least64_t spinlock;
  struct defer_node *deferList;

  /*
    Recycled defer nodes that can be reused. This is not a thread-safe field and
    only be accessed by the thread that owns this node.
  */
  struct defer_node *recycleList;

  /*
    Next TLS node. Note that this field can be updated atomically by newer threads
    and so should be accessed via atomics.
  */
  struct tls_node *next;
};

// List of thread-local data.
static struct tls_node *chpl_qsbr_tls_list;
static CHPL_TLS_DECL(struct tls_node *, chpl_qsbr_tls);

// As multiple threads can register themselves at a later time using atomic 
// RMW operations, all loads of the list must also be atomic.
static inline struct tls_node *get_tls_list(void);
static inline struct tls_node *get_tls_list(void) {
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
  return atomic_fetch_add_uint_least64_t(&global_epoch, 1);
}

// Update a thread's current epoch to the most recent; this
// is a full memory barrier operation
static inline void observe_epoch(struct tls_node *node);
static inline void observe_epoch(struct tls_node *node) {
  atomic_store_uint_least64_t(&node->epoch, get_global_epoch());
}

static inline uint64_t get_tasks(struct tls_node *node);
static inline uint64_t get_tasks(struct tls_node *node) {
  return atomic_load_uint_least64_t(&node->nTasks);
}

static inline uint64_t add_task(struct tls_node *node);
static inline uint64_t add_task(struct tls_node *node) {
  return atomic_fetch_add_uint_least64_t(&node->nTasks, 1);
}

static inline uint64_t remove_task(struct tls_node *node);
static inline uint64_t remove_task(struct tls_node *node) {
  return atomic_fetch_sub_uint_least64_t(&node->nTasks, 1);
}

static inline void acquire_spinlock(struct tls_node *node);
static inline void acquire_spinlock(struct tls_node *node) {
  // Fast path: Quick acquire spinlock
  if (atomic_exchange_uint_least64_t(&node->spinlock, 1) == 1) {
    // Slow path: TestAndTestAndSet. Note that we cannot yield or
    // we will end up calling the checkpoint.
    while (true) {
      uint64_t spinlock = atomic_load_uint_least64_t(&node->spinlock);
      if (spinlock == 0 
        && atomic_compare_exchange_weak_uint_least64_t(&node->spinlock, 0, 1)) {
        break;
      }
    }
  }
}

static inline void release_spinlock(struct tls_node *node);
static inline void release_spinlock(struct tls_node *node) {
  atomic_store_uint_least64_t(&node->spinlock, 0);
}

// Requires spinlock.
static inline struct defer_node *get_defer_list(struct tls_node *node);
static inline struct defer_node *get_defer_list(struct tls_node *node) {
  return node->deferList;
}

// Requires spinlock. Pops all items less than or equal to 'epoch'.
static inline struct defer_node *pop_bulk_defer_list(struct tls_node *node, uint64_t epoch);
static inline struct defer_node *pop_bulk_defer_list(struct tls_node *node, uint64_t epoch) {
  struct defer_node *dnode = node->deferList, *prev = NULL;
  while (dnode != NULL) {
    if (epoch > dnode->targetEpoch) {
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
  struct defer_node *head = node->deferList;
  node->deferList = NULL;
  return head;
}

// Requires spinlock.
static inline void push_defer_list(struct tls_node *node, struct defer_node *dnode);
static inline void push_defer_list(struct tls_node *node, struct defer_node *dnode) {
  dnode->next = node->deferList;
  node->deferList = dnode;
}

// Must be called from the thread owning the node.
static inline struct defer_node *pop_recycle_list(struct tls_node *node);
static inline struct defer_node *pop_recycle_list(struct tls_node *node) {
  struct defer_node *dnode = node->recycleList;
  
  // If empty, return fresh allocation
  if (dnode == NULL) {
    return chpl_mem_calloc(1, sizeof(struct defer_node), 0, 0, 0);
  }

  node->recycleList = dnode->next;
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
static void init_tls(void);
static void init_tls(void) {
  struct tls_node *node = chpl_calloc(1, sizeof(struct tls_node));
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

// Obtains the minimum epoch of all threads.
static uint64_t safe_epoch(void);
static uint64_t safe_epoch(void) {
  uint64_t min = (uint64_t) -1;
  // Check all remaining threads for whether they have updated to a more recent epoch.
  for (struct tls_node *node = get_tls_list(); node != NULL; node = node->next) {
    if (get_tasks(node) > 0) {
      uint64_t epoch = get_epoch(node);
      min = min ? (min < epoch) : epoch;       
    }
  }
  
  return min;
}

// Requires spinlock.
static inline void handle_deferred_data(struct tls_node *node);
static inline void handle_deferred_data(struct tls_node *node) {
  // Acquire all data that can be deleted.
  uint64_t epoch = safe_epoch();
  struct defer_node *list = pop_bulk_defer_list(node, epoch);

  // Handle deletion of said data...
  for (struct defer_node *dnode = list; dnode != NULL;) {
    struct defer_node *tmp = dnode;
    dnode = dnode->next;
    delete_data(tmp);
    push_recycle_list(node, tmp);
  }
}

void chpl_qsbr_init(void) {
    CHPL_TLS_INIT(chpl_qsbr_tls);
}

void chpl_qsbr_unblocked(void) {
  struct tls_node *tls = CHPL_TLS_GET(chpl_qsbr_tls);
  if (tls == NULL) {
    init_tls();
    tls = CHPL_TLS_GET(chpl_qsbr_tls);
  }

  // Notify that we are now in-use.
  atomic_store_uint_least64_t(&tls->nTasks, 1);
}


void chpl_qsbr_blocked(void) {
  struct tls_node *tls = CHPL_TLS_GET(chpl_qsbr_tls);
  if (tls == NULL) {
    init_tls();
    tls = CHPL_TLS_GET(chpl_qsbr_tls);
  }

  atomic_store_uint_least64_t(&tls->nTasks, 0);

  acquire_spinlock(tls);

  // Empty as best we can
  handle_deferred_data(tls);
  if (tls->deferList == NULL) {
    release_spinlock(tls);
    return;
  }

  // At this point if we have any deferred data, we must pass this to some
  // other thread, as we could be parked indefinitely. Find first active
  // thread to dump the rest of our work on...
  struct tls_node *node = get_tls_list();
  for (; node != NULL; node = node->next) {
    if (get_tasks(node) > 0) {
      // Double check...
      acquire_spinlock(node);
      if (get_tasks(node) > 0) {
        break;
      }
      release_spinlock(node);  
    }
  }

  // If node is NULL, then all threads are finished so we should be able to safely
  // handle our deferred data...
  if (node == NULL) {
    handle_deferred_data(tls);
  } else {
    // If the node is not NULL, we found our thread to dump on and we already hold its lock.
    struct defer_node *list = pop_all_defer_list(tls);
    
    // If their list is empty its easy
    if (node->deferList == NULL) {
      node->deferList = list;
    } else {
      // TODO: Optimize! If tls->deferList is size N and node->deferList is size M,
      // then is O((M+N) * N) = O(M*N^2).
      while (list != NULL) {
        struct defer_node *dnode = list;
        list = list->next;

        // Insert as head
        if (dnode->targetEpoch >= node->deferList->targetEpoch) {
          dnode->next = node->deferList;
          node->deferList = dnode;
          continue;
        }

        // Insert between nodes...
        struct defer_node *ddnode = node->deferList, *prev = NULL;
        for (; ddnode != NULL; ddnode = ddnode->next) {
          if (ddnode->targetEpoch >= dnode->targetEpoch) {
            assert(prev != NULL);
            dnode->next = ddnode->next;
            ddnode->next = dnode;
            break;
          }

          prev = ddnode;
        }

        // Exhausted all options, make as tail...
        if (ddnode == NULL) {
          assert(prev != NULL);
          prev->next = dnode;
        }
      }
    }

    release_spinlock(node);
  }

  release_spinlock(tls);
}

void chpl_qsbr_checkpoint(void) {
    struct tls_node *tls = CHPL_TLS_GET(chpl_qsbr_tls);
    if (tls == NULL) {
        init_tls();
        tls = CHPL_TLS_GET(chpl_qsbr_tls);
    }

    // Observe the current epoch.
    observe_epoch(tls);

    // Check if we have passed enough checkpoints to process our deferred data.
    tls->passedCheckpoints++;
    if (tls->passedCheckpoints % CHPL_QSBR_CHECKPOINTS_PER_PROCESS == 0
      && tls->deferList != NULL) {
      acquire_spinlock(tls);
      handle_deferred_data(tls);
      release_spinlock(tls);
    } 
}

void chpl_qsbr_defer_deletion(void *data) {
    uint64_t epoch = advance_global_epoch() + 1;
    struct tls_node *tls = CHPL_TLS_GET(chpl_qsbr_tls);
    if (tls == NULL) {
      init_tls();
      tls = CHPL_TLS_GET(chpl_qsbr_tls);
    }
    observe_epoch(tls);

    struct defer_node *dnode = pop_recycle_list(tls);
    dnode->targetEpoch = epoch;
    dnode->data = data;
    dnode->numData = 0;

    acquire_spinlock(tls);
    push_defer_list(tls, dnode);
    release_spinlock(tls);
}

void chpl_qsbr_defer_deletion_multi(void **arrData, int numData) {
    uint64_t epoch = advance_global_epoch() + 1;
    struct tls_node *tls = CHPL_TLS_GET(chpl_qsbr_tls);
    if (tls == NULL) {
      init_tls();
      tls = CHPL_TLS_GET(chpl_qsbr_tls);
    }
    observe_epoch(tls);

    struct defer_node *dnode = pop_recycle_list(tls);
    dnode->targetEpoch = epoch;
    dnode->data = arrData;
    dnode->numData = numData;

    acquire_spinlock(tls);
    push_defer_list(tls, dnode);
    release_spinlock(tls);
}

void chpl_qsbr_exit(void) {
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
