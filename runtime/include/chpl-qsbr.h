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

void chpl_qsbr_init(void);

// Called periodically to declare a quiescent state
void chpl_qsbr_checkpoint(void);

// Defer deletion of the data until it is safe.
void chpl_qsbr_defer_deletion(void *data);

// Variant of 'chpl_qsbr_defer_deletion' that accepts an array of data.
void chpl_qsbr_defer_deletion_multi(void **arrData, int numData);

// Must be called when a thread will be blocked may be unable to call a checkpoint
// for a time. If a thread is marked as 'blocked' it must not access QSBR-protected
// data or else it is unsafe.
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

// Registers a thread to use QSBR. 
void chpl_qsbr_register(void);

void chpl_qsbr_exit(void);

#endif // _chpl_qsbr_h_
