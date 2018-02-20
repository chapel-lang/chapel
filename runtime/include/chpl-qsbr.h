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

#include <stdint.h>
#include <stdbool.h>
#include "chpl-atomics.h"
#include "chpl-thread-local-storage.h"
#include "chpl-env.h"

#ifndef _chpl_qsbr_h_
#define _chpl_qsbr_h_

/*
 Concurrent-safe memory reclamation using Quiescent State-Based Reclamation (QSBR).

 For memory to be appropriately protected by QSBR, two invariants must be established:
  1.  References to QSBR-protected memory obtained prior to a checkpoint must not be
      used after the checkpoint, otherwise the reference is unsafe.
  2.  QSBR-protected memory must only be deferred for deletion at most once.

 The idiomatic usage of QSBR involves separating memory reclamation into two phases:
  1.  Logically deleting memory by removing it from some data structure in some linearizable fashion.
  2.  Physically deleting the memory by deferring deletion of it.

 This is commonly the case for non-blocking data structures and algorithms. For an example, see below:

 ======================================
      Non-Blocking Stack
 ======================================
  struct node *head;

  void *dequeue(void) {
    struct node *currentHead, *newHead;
    // The currentHead is logically removed from the list...
    do {
      currentHead = load(&head);
      if (currentHead == NULL) return NULL;
      newHead = currentHead->next;
    } while (CAS(&head, currentHead, newHead));
    
    // Queue up for physical removal...
    void *ret = currentHead->value;
    chpl_qsbr_defer_delete(currentHead);
    return ret;
  }
*/

static const int CHPL_QSBR_ITERATIONS_PER_CHECKPOINT = 64;

// Thread-specific meta data.
struct tls_node {
  /*
    The current observed epoch. This is used to ensure that writers
    do not delete an older instance that corresponds to this epoch.
  */
  atomic_uint_least64_t epoch;
  /*
    Whether or not this thread is parked in the runtime. If the thread is
    parked, it is ignored during memory reclamation checks.
  */
  atomic_uint_least64_t parked;
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

extern CHPL_TLS_DECL(struct tls_node *, chpl_qsbr_tls);

#define chpl_qsbr_likely(x)       __builtin_expect((x),1)
#define chpl_qsbr_unlikely(x)     __builtin_expect((x),0)

extern void chpl_qsbr_init_tls(void);
extern int chpl_qsbr_is_enabled;
extern pthread_once_t chpl_qsbr_once_flag;

static inline void chpl_qsbr_get_enabled(void) {
  chpl_qsbr_is_enabled = chpl_env_rt_get_bool("QSBR", true);
}

static inline chpl_bool chpl_qsbr_enabled(void) {
  if (chpl_qsbr_is_enabled == -1) {
    pthread_once(&chpl_qsbr_once_flag, chpl_qsbr_get_enabled);
  }

  return chpl_qsbr_is_enabled;
}

// Performs a check to ensure that the current thread is registered
// for QSBR, and if not then it will handle registration. On average takes ~1 nanosecond.
static inline void chpl_qsbr_quickcheck(void) {
  if (chpl_qsbr_enabled() && chpl_qsbr_unlikely(CHPL_TLS_GET(chpl_qsbr_tls) == NULL)) {
    chpl_qsbr_init_tls();
  }
}

void chpl_qsbr_init(void);

// Declares the current thread as being in a quiescent-state; that is, that
// it does not have any access to any QSBR-protected data. It is erroneous
// to use a reference to some QSBR-protected data after invoking a checkpoint
// as the reference could have been reclaimed. A checkpoint should be invoked
// where it is known that no task multiplexed to the same thread will access
// any aforementioned data. It is recommended that no task switching (that is
// calls to `chpl_task_yield` or acquiring a lock) should take place while
// intending to use any QSBR-protected data. If QSBR is disabled no data after
// to the point of which it was disabled will be reclaimed, but data inserted
// prior will be elible for memory reclamation. On average takes ~30 nanoseconds.
void chpl_qsbr_checkpoint(void);

// Defer deletion of the data until it is safe. No data is reclaimed until a checkpoint
// is invoked, so strategic calls to the a checkpoint is required to avoid memory leakage.
void chpl_qsbr_defer_deletion(void *data);

// Variant of 'chpl_qsbr_defer_deletion' that accepts an array of data.
void chpl_qsbr_defer_deletion_multi(void **arrData, int numData);

// Invoked when a thread (Note: *Not just a task*) will be unable to invoke checkpoints.
// A blocked thread will not count when determining whether data can be reclaimed, and as
// such a blocked thread should never access QSBR-protected data. 
void chpl_qsbr_blocked(void);
void chpl_qsbr_unblocked(void);

// Enables/Disables QSBR. When disabled, no memory deferred for deletion after the current
// safest epoch will be reclaimed, but memory deferred for deletion prior to the safest is
// still eligible for reclamation. Note that this is disabled/enables QSBR for the *entire locale*
// and as such care must be taken so that it is only called by a single coordinating thread. After
// being enabled, memory reclamation proceeds as normal. Memory deferred for deletion while disabled
// is safe but will not reclaimed until enabled.
void chpl_qsbr_disable(void);
void chpl_qsbr_enable(void);

void chpl_qsbr_exit(void);

#endif // _chpl_qsbr_h_
