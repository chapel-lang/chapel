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
#include "chpl-privatization.h"
#include "chpl-atomics.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include <pthread.h>

static chpl_sync_aux_t privatizationSync;
static volatile uint64_t global_epoch = 0;

void** chpl_privateObjects = NULL;
static int64_t chpl_capPrivateObjects = 0;

// Thread-local state that keeps track of the current version of
// 'chpl_privatizedObjects'. This adheres to the Quiescent State-Based Reclamation
// strategy used in certain RCU implementations.  
struct tls_node {
  volatile uint64_t epoch;
  volatile uint64_t nTasks;
  volatile uint64_t nWriters;
  struct tls_node *next;
};

// List of thread-local states.
static struct tls_node *tls_list;
static CHPL_TLS_DECL(struct tls_node *, reader_tls);

// Initializes TLS; should only need to be called once.
static void init_tls(void);
static void init_tls(void) {  
  struct tls_node *node = chpl_calloc(1, sizeof(*node));
  node->epoch = global_epoch;

  // Append to head of list
  struct tls_node *old_head;
  do {
    old_head = tls_list;
    node->next = old_head;
  } while (!atomic_compare_exchange_weak_uintptr_t((uintptr_t *) &tls_list, (uintptr_t) old_head, (uintptr_t) node));
  
  CHPL_TLS_SET(reader_tls, node);
}

void chpl_privatization_init(void) {
    chpl_sync_initAux(&privatizationSync);
    CHPL_TLS_INIT(reader_tls);
    
    // Initialize...
    chpl_capPrivateObjects = 1024;
    chpl_privateObjects =
        chpl_mem_allocManyZero(chpl_capPrivateObjects, sizeof(void *),
                           CHPL_RT_MD_COMM_PRV_OBJ_ARRAY, 0, 0);
}

// Called when we are sure we no longer have access to 'chpl_privatizedObjects'
void chpl_privatization_checkpoint(void);
void chpl_privatization_checkpoint(void) {
  struct tls_node *node = CHPL_TLS_GET(reader_tls);
  if (node == NULL) {
    init_tls();
    node = CHPL_TLS_GET(reader_tls);
  }

  /* Observe the current epoch. */
  node->epoch = global_epoch;
}

void chpl_privatization_incr(void); 
void chpl_privatization_incr(void) {
  struct tls_node *node = CHPL_TLS_GET(reader_tls);
  if (node == NULL) {
    init_tls();
    node = CHPL_TLS_GET(reader_tls);
  }

  // Notify that we are now in-use.
  node->nTasks++;
}

void chpl_privatization_decr(void); 
void chpl_privatization_decr(void) {
  struct tls_node *node = CHPL_TLS_GET(reader_tls);
  if (node == NULL) {
    init_tls();
    node = CHPL_TLS_GET(reader_tls);
  }

  // Notify that we have one less task
  node->nTasks--;
}

static inline int64_t max(int64_t a, int64_t b) {
  return a > b ? a : b;
}

// Note that this function can be called in parallel and more notably it can be
// called with non-monotonic pid's. e.g. this may be called with pid 27, and
// then pid 2, so it has to ensure that the privatized array has at least pid+1
// elements. Be __very__ careful if you have to update it.
void chpl_newPrivatizedClass(void* v, int64_t pid) {
  struct tls_node *node = CHPL_TLS_GET(reader_tls);
  if (node == NULL) {
    init_tls();
    node = CHPL_TLS_GET(reader_tls);
  }
  node->nWriters++;
  
  chpl_sync_lock(&privatizationSync);
  // if we're out of space, double (or more) the array size
  if (pid >= chpl_capPrivateObjects) {
    void** tmp;
    void** old;
    int64_t oldCap;

    oldCap = chpl_capPrivateObjects;
    chpl_capPrivateObjects = 2*max(pid, oldCap);

    tmp = chpl_mem_allocManyZero(chpl_capPrivateObjects, sizeof(void *),
                                 CHPL_RT_MD_COMM_PRV_OBJ_ARRAY, 0, 0);
    chpl_memcpy((void*)tmp, (void*)chpl_privateObjects, (oldCap)*sizeof(void*));
    old = chpl_privateObjects;
    chpl_privateObjects = tmp;

    full_memory_barrier();
    global_epoch = global_epoch + 1;
    for (struct tls_node *node = tls_list; node != NULL; node = node->next) {
      // Wait for threads which have tasks that are not writers
      while (node->nTasks > node->nWriters && node->epoch != global_epoch) {
        chpl_task_yield();
      }
    }

    chpl_mem_free(old, 0, 0);
  }

  chpl_privateObjects[pid] = v;

  chpl_sync_unlock(&privatizationSync);
  node->nWriters--;
}

void chpl_clearPrivatizedClass(int64_t i) {
  struct tls_node *node = CHPL_TLS_GET(reader_tls);
  if (node == NULL) {
    init_tls();
    node = CHPL_TLS_GET(reader_tls);
  }
  node->nWriters++;

  chpl_sync_lock(&privatizationSync);
  chpl_privateObjects[i] = NULL;
  chpl_sync_unlock(&privatizationSync);

  node->nWriters--;
}

// Used to check for leaks of privatized classes
int64_t chpl_numPrivatizedClasses(void) {
  struct tls_node *node = CHPL_TLS_GET(reader_tls);
  if (node == NULL) {
    init_tls();
    node = CHPL_TLS_GET(reader_tls);
  }
  node->nWriters++;

  int64_t ret = 0;
  chpl_sync_lock(&privatizationSync);
  for (int64_t i = 0; i < chpl_capPrivateObjects; i++) {
    if (chpl_privateObjects[i])
      ret++;
  }
  chpl_sync_unlock(&privatizationSync);

  node->nWriters--;
  return ret;
}
 
// Clean up all resources for memory checkers.
void chpl_privatization_exit(void) {
  // Clean thread-local storage
  while (tls_list) {
    struct tls_node *node = tls_list;
    tls_list = tls_list->next;
    chpl_mem_free(node, 0, 0);
  }

  // Destroy last instance
  chpl_mem_free(chpl_privateObjects, 0, 0);
  chpl_privateObjects = NULL;
}