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

// Maximum number of deferred data to handle deletion of.
#define CHPL_QSBR_MAX_PROCESS 1024
// Number of checkpoints to pass before processing
#define CHPL_QSBR_CHECKPOINTS_PER_PROCESS 8



static atomic_uint_least64_t global_epoch = 0;

struct defer_data {
    void *data;
    int numData;
};

// Deferred deletion list and associated epoch
// Ideas: First thread that does not complete within a
// reasonable amount of time gets to handle deleting it.
// If a thread hits a checkpoint and seeing a deferred node,
// it will process one of them; if it is unable to, it will
// 'pass the buck' to the next problematic thread ensuring that
// eventually it will be deleted.
struct defer_node {
  uint64_t targetEpoch;
  struct defer_data ddata;
  struct defer_node *next;
};

// Thread-local data that keeps track of our observed global state.
struct tls_node {
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
    List of objects that are queued for deferred deletion. Each
    time a writer has waited for too long on a particular thread,
    it will 'pass the buck' for deletion to the next thread.
  */
  struct defer_node *deferList;
  /*
    Recycled defer nodes that can be reused.
  */
  struct defer_node *recycleList;
  struct tls_node *next;
};

// List of thread-local data.
static struct tls_node *chpl_qsbr_tls_list;
static CHPL_TLS_DECL(struct tls_node *, chpl_qsbr_tls);

static inline struct tls_node *get_tls_list() {
  return (struct tls_node *) atomic_load_uintptr_t((uintptr_t *) &chpl_qsbr_tls_list);
}

static inline uint64_t get_epoch(struct tls_node *node) {
  return atomic_load_uint_least64_t(&node->epoch);
}

static inline void set_epoch(struct tls_node *node, uint64_t epoch) {
  return atomic_store_uint_least64_t(&node->epoch, epoch);
}

static inline uint64_t get_global_epoch() {
  return atomic_load_uint_least64_t(&global_epoch);
}

static inline uint64_t advance_global_epoch() {
  return atomic_fetch_add_uint_least64_t(&global_epoch, 1);
}

static inline uint64_t get_tasks(struct tls_node *node) {
  return atomic_load_uint_least64_t(&node->nTasks);
}

static inline uint64_t add_task(struct tls_node *node) {
  return atomic_fetch_add_uint_least64_t(&node->nTasks, 1);
}

static inline uint64_t remove_task(struct tls_node *node) {
  return atomic_fetch_sub_uint_least64_t(&node->nTasks, 1);
}

static inline struct defer_node *get_defer_list(struct tls_node *node) {
  return (struct defer_node *) atomic_load_uintptr_t((uintptr_t *) &node->deferList);
}

static inline struct defer_node *pop_all_defer_list(struct tls_node *node) {
  return (struct defer_node *) atomic_exchange_uintptr_t((uintptr_t *) &node->deferList, NULL);
}

static inline struct defer_node *pop_defer_list(struct tls_node *node) {
  struct defer_node *dnode;
  do {
    dnode = get_defer_list(node);
    if (dnode == NULL) {
      return NULL;
    }
  } while (!atomic_compare_exchange_weak_uintptr_t(
      (uintptr_t *) &node->deferList, (uintptr_t) dnode, (uintptr_t) dnode->next)
    );

  return dnode;
}

static inline void push_defer_list(struct tls_node *node, struct defer_node *dnode) {
  struct defer_node *old_head;
  do {
      old_head = get_defer_list(node);
      dnode->next = old_head;
  } while (!atomic_compare_exchange_weak_uintptr_t(
      (uintptr_t *) &node->deferList, (uintptr_t) old_head, (uintptr_t) dnode)
    );
}

static inline struct defer_node *get_recycle_list(struct tls_node *node) {
  return (struct defer_node *) atomic_load_uintptr_t((uintptr_t *) &node->recycleList);
}

static inline struct defer_node *pop_recycle_list(struct tls_node *node) {
  struct defer_node *dnode;
  do {
    dnode = get_recycle_list(node);

    // TODO: Maybe allocate in bulk with 'chpl_mem_allocMany'?
    // If empty, allocate a new one.
    if (dnode == NULL) {
      return chpl_mem_calloc(1, sizeof(*defer), 0, 0, 0);
    }
  } while (!atomic_compare_exchange_weak_uintptr_t(
      (uintptr_t *) &node->recycleList, (uintptr_t) dnode, (uintptr_t) dnode->next)
    );

  return dnode;
}

static inline void push_recycle_list(struct tls_node *node, struct defer_node *dnode) {
  struct defer_node *old_head;
  do {
      old_head = get_recycle_list(node);
      dnode->next = old_head;
  } while (!atomic_compare_exchange_weak_uintptr_t(
      (uintptr_t *) &node->recycleList, (uintptr_t) old_head, (uintptr_t) dnode)
    );
}

// Initializes TLS; should only need to be called once.
static void init_tls(void);
static void init_tls(void) {
  struct tls_node *node = chpl_calloc(1, sizeof(*node));
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


// Check for if all threads
static bool defer_deletion(uint64_t targetEpoch, struct defer_data ddata);
static bool defer_deletion(uint64_t targetEpoch, struct defer_data ddata) {
  // Check all remaining threads for whether they have updated to a more recent epoch.
  for (struct tls_node *node = get_tls_list(); node != NULL; node = node->next) {
    if (get_tasks(node) > 0 && get_epoch(node) < targetEpoch) {      
        return false;
    }
  }

  // Treat as an array?
  if (ddata.numData) {
      void **arr = ddata.data;
      for (int i = 0; i < ddata.numData; i++) {
          chpl_mem_free(arr[i], 0, 0);
      }
  }
  chpl_mem_free(ddata.data, 0, 0);

  return true;
}

void chpl_qsbr_init(void) {
    CHPL_TLS_INIT(chpl_qsbr_tls);
}

void chpl_qsbr_onTaskCreation(void) {
  struct tls_node *node = CHPL_TLS_GET(chpl_qsbr_tls);
  if (node == NULL) {
    init_tls();
    node = CHPL_TLS_GET(chpl_qsbr_tls);
  }

  // Notify that we are now in-use.
  node->nTasks++;
}


void chpl_qsbr_onTaskDestruction(void) {
  struct tls_node *tls = CHPL_TLS_GET(chpl_qsbr_tls);
  if (tls == NULL) {
    init_tls();
    tls = CHPL_TLS_GET(chpl_qsbr_tls);
  }

  // Notify that we have one less task
  uint64_t nTasks = atomic_fetch_sub_uint_least64_t(&tls->nTasks, 1);

  // If we are the last task, empty our defer list...
  while (nTasks == 1 && tls->deferList) {
    // Pop
    struct defer_node *dnode;
    do {
      dnode = (struct defer_node *) atomic_load_uintptr_t((uintptr_t *) &tls->deferList);
    } while (!atomic_compare_exchange_weak_uintptr_t((uintptr_t *) &tls->deferList, (uintptr_t) dnode, (uintptr_t) dnode->next));

    // Only process threads that come after us as a previous thread would have been processed
    // before us and newly registered threads are pushed as the head of the tls_list.
    defer_deletion(dnode->targetEpoch, chpl_qsbr_tls_list, dnode->ddata);
    chpl_mem_free(dnode, 0, 0);
  }
}

void chpl_qsbr_checkpoint(void) {
    struct tls_node *tls = CHPL_TLS_GET(chpl_qsbr_tls);
    if (tls == NULL) {
        init_tls();
        tls = CHPL_TLS_GET(chpl_qsbr_tls);
    }

    // Observe the current epoch.
    atomic_store_uint_least64_t(&tls->epoch, atomic_load_uint_least64_t(&global_epoch));

    // Handle a single deferred object we have, if any
    struct defer_node *defer_list = (struct defer_node *) 
      atomic_exchange_uintptr_t((uintptr_t *) &tls->deferList, NULL);

    if (defer_list) {
        // Pop
        struct defer_node *dnode;
        do {
            dnode = (struct defer_node *) atomic_load_uintptr_t((uintptr_t *) &tls->deferList);
        } while (!atomic_compare_exchange_weak_uintptr_t(
            (uintptr_t *) &tls->deferList, (uintptr_t) dnode, (uintptr_t) dnode->next)
          );

        // Only process threads that come after us as a previous thread would have been processed
        // before us and newly registered threads are pushed as the head of the tls_list.
        defer_deletion(
          dnode->targetEpoch, 
          (struct tls_node *) atomic_load_uintptr_t((uintptr_t *) &chpl_qsbr_tls_list), 
          dnode->ddata
        );
        chpl_mem_free(dnode, 0, 0);
    }

    full_memory_barrier();
}

void chpl_qsbr_defer_deletion(void *data) {
    uint64_t epoch = atomic_fetch_add_uint_least64_t(&global_epoch, 1) + 1;
    struct tls_node *node = CHPL_TLS_GET(chpl_qsbr_tls);
    if (node == NULL) {
      init_tls();
      node = CHPL_TLS_GET(chpl_qsbr_tls);
    }
    atomic_store_uint_least64_t(&node->epoch, epoch);

    defer_deletion(
      epoch, 
      (struct tls_node *) atomic_load_uintptr_t((uintptr_t *) &chpl_qsbr_tls_list), 
      (struct defer_data) { .data = data, .numData = 0 }
    );
}

void chpl_qsbr_defer_deletion_multi(void **arrData, int numData) {
    uint64_t epoch = atomic_fetch_add_uint_least64_t(&global_epoch, 1) + 1;
    struct tls_node *node = CHPL_TLS_GET(chpl_qsbr_tls);
    if (node == NULL) {
      init_tls();
      node = CHPL_TLS_GET(chpl_qsbr_tls);
    }
    node->epoch = epoch;

    defer_deletion(
      epoch, 
      (struct tls_node *) atomic_load_uintptr_t((uintptr_t *) &chpl_qsbr_tls_list), 
      (struct defer_data) { .data = arrData, .numData = numData}
    );
}

void chpl_qsbr_exit(void) {
    // Clean thread-local storage
    while (chpl_qsbr_tls_list) {
        struct tls_node *node = chpl_qsbr_tls_list;
        chpl_qsbr_tls_list = chpl_qsbr_tls_list->next;

        while (node->deferList) {
            struct defer_node *dnode = node->deferList;
            node->deferList = node->deferList->next;

            if (dnode->ddata.numData) {
                void **arr = dnode->ddata.data;
                for (int i = 0; i < dnode->ddata.numData; i++) {
                    chpl_mem_free(arr[i], 0, 0);
                }
            }

            chpl_mem_free(dnode->ddata.data, 0, 0);
            chpl_mem_free(dnode, 0, 0);
        }

        chpl_mem_free(node, 0, 0);
    }
}
