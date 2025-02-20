/* System Headers */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

/* Public Headers */
#include "qthread/cacheline.h"
#include "qthread/qthread.h"

/* Internal Headers */
#include "qt_alloc.h"
#include "qt_asserts.h"
#include "qt_envariables.h"
#include "qt_expect.h"
#include "qt_prefetch.h"
#include "qt_qthread_mgmt.h"
#include "qt_qthread_struct.h"
#include "qt_shepherd_innards.h"
#include "qt_subsystems.h"
#include "qt_threadqueue_scheduler.h"
#include "qt_threadqueues.h"
#include "qt_visibility.h"
#include "qthread_innards.h" /* for qlib */

typedef struct qt_threadqueue_node_s qt_threadqueue_node_t;

/* Data Structures */
struct qt_threadqueue_node_s {
  struct qt_threadqueue_node_s *next;
  struct qt_threadqueue_node_s *prev;
  uintptr_t stealable;
  qthread_t *value;
};

struct _qt_threadqueue {
  qt_threadqueue_node_t *head;
  qt_threadqueue_node_t *tail;
  _Atomic long qlength;
  _Atomic long qlength_stealable; /* number of stealable tasks on queue - stop
                                   * steal attempts that will fail because
                                   * tasks cannot be moved - 4/1/11 AKP
                                   */
  QTHREAD_TRYLOCK_TYPE qlock;
} /* qt_threadqueue_t */;

static aligned_t steal_disable = 0;
static long steal_chunksize = 0;

// Forward declarations
qt_threadqueue_node_t INTERNAL *
qt_threadqueue_dequeue_steal(qt_threadqueue_t *h, qt_threadqueue_t *v);

void INTERNAL qt_threadqueue_enqueue_multiple(qt_threadqueue_t *q,
                                              qt_threadqueue_node_t *first);

/* Memory Management */
qt_threadqueue_pools_t generic_threadqueue_pools;
#define ALLOC_THREADQUEUE()                                                    \
  (qt_threadqueue_t *)qt_mpool_alloc(generic_threadqueue_pools.queues)
#define FREE_THREADQUEUE(t) qt_mpool_free(generic_threadqueue_pools.queues, t)

static inline qt_threadqueue_node_t *ALLOC_TQNODE(void) {
  return (qt_threadqueue_node_t *)qt_mpool_alloc(
    generic_threadqueue_pools.nodes);
}

#define FREE_TQNODE(t) qt_mpool_free(generic_threadqueue_pools.nodes, t)

static void qt_threadqueue_subsystem_shutdown(void) {
  qt_mpool_destroy(generic_threadqueue_pools.nodes);
  qt_mpool_destroy(generic_threadqueue_pools.queues);
}

void INTERNAL qt_threadqueue_subsystem_init(void) {
  generic_threadqueue_pools.queues =
    qt_mpool_create_aligned(sizeof(qt_threadqueue_t), qthread_cacheline());
  generic_threadqueue_pools.nodes =
    qt_mpool_create_aligned(sizeof(qt_threadqueue_node_t), qthread_cacheline());
  steal_chunksize = qt_internal_get_env_num("STEAL_CHUNK", 0, 0);
  qthread_internal_cleanup(qt_threadqueue_subsystem_shutdown);
}

ssize_t INTERNAL qt_threadqueue_advisory_queuelen(qt_threadqueue_t *q) {
  return atomic_load_explicit(&q->qlength, memory_order_relaxed);
}

/*****************************************/
/* functions to manage the thread queues */
/*****************************************/

static inline qt_threadqueue_node_t *
qthread_steal(qthread_shepherd_t *thief_shepherd);

qt_threadqueue_t INTERNAL *qt_threadqueue_new(void) {
  qt_threadqueue_t *q = ALLOC_THREADQUEUE();

  if (q != NULL) {
    q->head = NULL;
    q->tail = NULL;
    atomic_store_explicit(&q->qlength, 0, memory_order_relaxed);
    atomic_store_explicit(&q->qlength_stealable, 0, memory_order_relaxed);
    QTHREAD_TRYLOCK_INIT(q->qlock);
  }

  return q;
}

extern qt_mpool generic_qthread_pool;
#define ALLOC_QTHREAD() (qthread_t *)qt_mpool_alloc(generic_qthread_pool)
#define FREE_QTHREAD(t) qt_mpool_free(generic_qthread_pool, t)

void INTERNAL qt_threadqueue_free(qt_threadqueue_t *q) {
  if (q->head != q->tail) {
    qthread_t *t;
    QTHREAD_TRYLOCK_LOCK(&q->qlock);
    while (q->head != q->tail) {
      qt_threadqueue_node_t *node = q->tail;
      if (node != NULL) {
        q->tail = node->prev;
        if (q->tail == NULL) {
          q->head = NULL;
        } else {
          q->tail->next = NULL;
        }
        t = node->value;
        FREE_TQNODE(node);
        FREE_QTHREAD(t);
      }
    }
    assert(q->head == NULL);
    assert(q->tail == NULL);
    atomic_store_explicit(&q->qlength, 0, memory_order_relaxed);
    atomic_store_explicit(&q->qlength_stealable, 0, memory_order_relaxed);
    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
  }
  assert(q->head == q->tail);
  QTHREAD_TRYLOCK_DESTROY(q->qlock);
  FREE_THREADQUEUE(q);
}

static inline int qt_threadqueue_isstealable(qthread_t *t) {
  return ((atomic_load_explicit(&t->flags, memory_order_relaxed) &
           QTHREAD_UNSTEALABLE) == 0)
           ? 1
           : 0;
}

/* enqueue at tail */
void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t *restrict q,
                                     qthread_t *restrict t) {
  qt_threadqueue_node_t *node;

  node = ALLOC_TQNODE();
  assert(node != NULL);

  node->value = t;
  node->stealable = qt_threadqueue_isstealable(t);

  assert(q != NULL);
  assert(t != NULL);

  QTHREAD_TRYLOCK_LOCK(&q->qlock);
  node->next = NULL;
  node->prev = q->tail;
  q->tail = node;
  if (q->head == NULL) {
    q->head = node;
  } else {
    node->prev->next = node;
  }
  // q->qlength++;
  atomic_fetch_add_explicit(&q->qlength, 1, memory_order_relaxed);
  // q->qlength_stealable += node->stealable;
  atomic_fetch_add_explicit(
    &q->qlength_stealable, node->stealable, memory_order_relaxed);
  QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
}

/* yielded threads enqueue at head */
void INTERNAL qt_threadqueue_enqueue_yielded(qt_threadqueue_t *restrict q,
                                             qthread_t *restrict t) {
  qt_threadqueue_node_t *node;

  node = ALLOC_TQNODE();
  assert(node != NULL);

  node->value = t;
  node->stealable = qt_threadqueue_isstealable(t);

  assert(q != NULL);
  assert(t != NULL);

  QTHREAD_TRYLOCK_LOCK(&q->qlock);
  node->prev = NULL;
  node->next = q->head;
  q->head = node;
  if (q->tail == NULL) {
    q->tail = node;
  } else {
    node->next->prev = node;
  }
  // q->qlength++;
  atomic_fetch_add_explicit(&q->qlength, 1, memory_order_relaxed);
  if (node->stealable) {
    // q->qlength_stealable++;
    atomic_fetch_add_explicit(&q->qlength_stealable, 1, memory_order_relaxed);
  }
  QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
}

/* dequeue at tail */
qthread_t INTERNAL *qt_scheduler_get_thread(qt_threadqueue_t *q,
                                            uint_fast8_t active) {
  qthread_shepherd_t *my_shepherd = qthread_internal_getshep();
  qthread_t *t;
  qthread_worker_id_t worker_id = NO_WORKER;

  assert(q != NULL);
  assert(my_shepherd);
  assert(my_shepherd->ready == q);
  assert(my_shepherd->sorted_sheplist);

  while (1) {
    qt_threadqueue_node_t *node = NULL;

    if (q->head) {
      QTHREAD_TRYLOCK_LOCK(&q->qlock);
      node = q->tail;
      if (node != NULL) {
        assert(q->head);
        assert(atomic_load_explicit(&q->qlength, memory_order_relaxed) > 0);

        q->tail = node->prev;
        if (q->tail == NULL) {
          q->head = NULL;
        } else {
          q->tail->next = NULL;
        }
        assert(atomic_load_explicit(&q->qlength, memory_order_relaxed) > 0);
        atomic_fetch_sub_explicit(&q->qlength, 1, memory_order_relaxed);
        atomic_fetch_sub_explicit(
          &q->qlength_stealable, node->stealable, memory_order_relaxed);
      }
      QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
    }

    if ((node == NULL) &&
        atomic_load_explicit(&my_shepherd->stealing, memory_order_relaxed)) {
      if (worker_id == NO_WORKER) { worker_id = qthread_worker(NULL); }
      if ((my_shepherd->shepherd_id == 0) && (worker_id == 0)) {
        while (atomic_load_explicit(&my_shepherd->stealing,
                                    memory_order_relaxed) == 1)
          SPINLOCK_BODY(); // no sense contending for the lock
      } else {
        while (
          atomic_load_explicit(&my_shepherd->stealing, memory_order_relaxed))
          SPINLOCK_BODY(); // no sense contending for the lock
      }
      continue;
    }

    if ((node == NULL) && (active)) {
      if (qlib->nshepherds > 1) {
        if (!steal_disable) {
          node = qthread_steal(my_shepherd);
        } else {
          while (NULL == q->head) SPINLOCK_BODY();
          continue;
        }
      }
    }
    if (node) {
      t = node->value;
      FREE_TQNODE(node);
      if ((atomic_load_explicit(&t->flags, memory_order_relaxed) &
           QTHREAD_REAL_MCCOY)) { // only needs to be on worker 0 for
                                  // termination
        if (worker_id == NO_WORKER) { worker_id = qthread_worker(NULL); }
        switch (worker_id) {
          case NO_WORKER:
            QTHREAD_TRAP(); // should never happen
            abort();
            continue; // keep looking
          case 0: {
            unsigned int expected = atomic_load_explicit(&my_shepherd->stealing,
                                                         memory_order_relaxed);
            while (expected && !atomic_compare_exchange_strong_explicit(
                                 &my_shepherd->stealing,
                                 &expected,
                                 0u,
                                 memory_order_relaxed,
                                 memory_order_relaxed)) {}
            return (t);
          }
          default:
            /* McCoy thread can only run on worker 0 */
            atomic_store_explicit(
              &my_shepherd->stealing, 2, memory_order_relaxed); // no stealing
            MACHINE_FENCE;
            qt_threadqueue_enqueue_yielded(q, t);
            continue; // keep looking
        }
      } else {
        break;
      }
    }
  }
  return (t);
}

/* enqueue multiple (from steal) */
void INTERNAL qt_threadqueue_enqueue_multiple(qt_threadqueue_t *q,
                                              qt_threadqueue_node_t *first) {
  qt_threadqueue_node_t *last;
  size_t addCnt = 1;

  assert(first != NULL);
  assert(q != NULL);

  last = first;
  while (last->next) {
    last = last->next;
    addCnt++;
  }

  QTHREAD_TRYLOCK_LOCK(&q->qlock);
  last->next = NULL;
  first->prev = q->tail;
  q->tail = last;
  if (q->head == NULL) {
    q->head = first;
  } else {
    first->prev->next = first;
  }
  atomic_fetch_add_explicit(&q->qlength, addCnt, memory_order_relaxed);
  atomic_fetch_add_explicit(
    &q->qlength_stealable, addCnt, memory_order_relaxed);
  QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
}

/* dequeue stolen threads at head, skip yielded threads */
qt_threadqueue_node_t INTERNAL *
qt_threadqueue_dequeue_steal(qt_threadqueue_t *h, qt_threadqueue_t *v) {
  qt_threadqueue_node_t *node;
  qt_threadqueue_node_t *first = NULL;
  qt_threadqueue_node_t *last = NULL;
  long amtStolen = 0;
  long desired_stolen;

  if (steal_chunksize == 0) {
    desired_stolen =
      atomic_load_explicit(&v->qlength_stealable, memory_order_relaxed) / 2;
  } else {
    desired_stolen = steal_chunksize;
  }

  assert(h != NULL);
  assert(v != NULL);

  if (desired_stolen == 0) { desired_stolen = 1; }

  if (!QTHREAD_TRYLOCK_TRY(&v->qlock)) { return NULL; }
  while (atomic_load_explicit(&v->qlength_stealable, memory_order_relaxed) >
           0 &&
         amtStolen < desired_stolen) {
    node = (qt_threadqueue_node_t *)v->head;
    do {
      // Find next stealable node (if one exists)
      while (node) {
        if (!node->stealable) {
          node = node->next;
        } else {
          break;
        }
      }
      if (node != NULL) {
        qt_threadqueue_node_t *first_stolen = node;
        qt_threadqueue_node_t *last_stolen = node;

        amtStolen++;

        // Adjust queue length(s)
        // v->qlength--;
        atomic_fetch_sub_explicit(&v->qlength, 1, memory_order_relaxed);
        // v->qlength_stealable--;
        atomic_fetch_sub_explicit(
          &v->qlength_stealable, 1, memory_order_relaxed);

        // Find next unstealable node, or amount we want to steal
        qt_threadqueue_node_t *next_to_steal = last_stolen->next;
        while (amtStolen < desired_stolen && next_to_steal) {
          if (!next_to_steal->stealable) {
            break;
          } else {
            last_stolen = next_to_steal;

            amtStolen++;

            // Adjust queue length(s)
            atomic_fetch_sub_explicit(&v->qlength, 1, memory_order_relaxed);
            atomic_fetch_sub_explicit(
              &v->qlength_stealable, 1, memory_order_relaxed);

            next_to_steal = next_to_steal->next;
          }
        }

        // Patch up the victim queue
        if (first_stolen == v->head) {
          v->head = last_stolen->next;
        } else {
          first_stolen->prev->next = last_stolen->next;
        }
        if (last_stolen == v->tail) {
          v->tail = first_stolen->prev;
        } else {
          last_stolen->next->prev = first_stolen->prev;
        }

        node = last_stolen->next;

        // Update steal list (first & last)
        first_stolen->prev = last_stolen->next = NULL;
        if (first == NULL) {
          first = first_stolen;
          last = last_stolen;
        } else {
          last->next = first_stolen;
          first_stolen->prev = last;
          last = last_stolen;
        }
      } else {
        break;
      }
    } while (atomic_load_explicit(&v->qlength_stealable, memory_order_relaxed) >
               0 &&
             amtStolen < desired_stolen);
    break;
  }
  QTHREAD_TRYLOCK_UNLOCK(&v->qlock);

  return (first);
}

/*  Steal work from another shepherd's queue
 *  Returns the work stolen
 */
static inline qt_threadqueue_node_t *
qthread_steal(qthread_shepherd_t *thief_shepherd) {
  qt_threadqueue_node_t *stolen = NULL;

  assert(thief_shepherd);

  if (atomic_load_explicit(&thief_shepherd->stealing, memory_order_relaxed)) {
    // this means that someone else on this shepherd is already stealing; I will
    // spin on my own queue.
    return NULL;
  } else {
#ifdef QTHREAD_OMP_AFFINITY
    if (thief_shepherd->stealing_mode == QTHREAD_STEAL_ON_ALL_IDLE) {
      int i;
      for (i = 0; i < qlib->nworkerspershep; i++)
        if (thief_shepherd->workers[i].current != NULL) { return NULL; }
      thief_shepherd->stealing_mode = QTHREAD_STEAL_ON_ANY_IDLE;
    }
#endif
    if (qthread_cas((unsigned int *)&thief_shepherd->stealing, 0, 1) !=
        0) { // avoid unnecessary stealing with a CAS
      return NULL;
    }
  }

  qthread_shepherd_id_t i = 0;
  qthread_shepherd_t *const shepherds = qlib->shepherds;
  qthread_shepherd_id_t *const sorted_sheplist =
    thief_shepherd->sorted_sheplist;
  assert(sorted_sheplist);

  qt_threadqueue_t *myqueue = thief_shepherd->ready;

  while (stolen == NULL) {
    qt_threadqueue_t *victim_queue = shepherds[sorted_sheplist[i]].ready;
    if (0 != atomic_load_explicit(&victim_queue->qlength_stealable,
                                  memory_order_relaxed)) {
      stolen = qt_threadqueue_dequeue_steal(myqueue, victim_queue);
      if (stolen) {
        qt_threadqueue_node_t *surplus = stolen->next;
        if (surplus) {
          stolen->next = NULL;
          surplus->prev = NULL;
          qt_threadqueue_enqueue_multiple(myqueue, surplus);
        }
        break;
      }
    }

    if ((0 < atomic_load_explicit(&myqueue->qlength, memory_order_relaxed)) ||
        steal_disable) { // work at home quit steal attempt
      break;
    }

    i++;
    i *= (i < qlib->nshepherds - 1);
    if (i == 0) { sched_yield(); }
    SPINLOCK_BODY();
  }
  atomic_store_explicit(&thief_shepherd->stealing, 0, memory_order_relaxed);
  return stolen;
}

/* walk queue removing all tasks matching this description */
void INTERNAL qt_threadqueue_filter(qt_threadqueue_t *q,
                                    qt_threadqueue_filter_f f) {
  qt_threadqueue_node_t *node = NULL;
  qthread_t *t = NULL;

  assert(q != NULL);
  /* For reference:
   *
   * dequeue (and filtering) starts at the tail and proceeds to follow the prev
   * ptrs until the head is reached.
   */

  QTHREAD_TRYLOCK_LOCK(&q->qlock);
  if (atomic_load_explicit(&q->qlength, memory_order_relaxed) > 0) {
    qt_threadqueue_node_t **lp = NULL;
    qt_threadqueue_node_t **rp = NULL;

    rp = &q->tail;
    node = q->tail;
    if (q->head == node) {
      lp = &q->head;
    } else {
      lp = &(node->prev->next);
    }
    while (node) {
      t = (qthread_t *)node->value;
      switch (f(t)) {
        case IGNORE_AND_CONTINUE: // ignore, move to the next one
          rp = &node->prev;
          node = node->prev;
          if (node) {
            if (q->head == node) {
              lp = &q->head;
            } else {
              lp = &(node->prev->next);
            }
          }
          break;
        case IGNORE_AND_STOP: // ignore, stop looking
          node = NULL;
          break;
        case REMOVE_AND_CONTINUE: // remove, move to the next one
        {
          qt_threadqueue_node_t *freeme;

          *lp = node->next;
          *rp = node->prev;
          // q->qlength--;
          // q->qlength_stealable -= node->stealable;
          atomic_fetch_sub_explicit(&q->qlength, 1, memory_order_relaxed);
          atomic_fetch_sub_explicit(
            &q->qlength_stealable, node->stealable, memory_order_relaxed);
          freeme = node;
          node = node->prev;
          if (q->head == node) {
            lp = &q->head;
          } else {
            lp = &(node->prev->next);
          }
          FREE_TQNODE(freeme);
          break;
        }
        case REMOVE_AND_STOP: // remove, stop looking
          *lp = node->next;
          *rp = node->prev;
          // q->qlength--;
          // q->qlength_stealable -= node->stealable;
          atomic_fetch_sub_explicit(&q->qlength, 1, memory_order_relaxed);
          atomic_fetch_sub_explicit(
            &q->qlength_stealable, node->stealable, memory_order_relaxed);
          FREE_TQNODE(node);
          node = NULL;
          break;
      }
    }
  }
  QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
}

/* walk queue looking for a specific value  -- if found remove it (and start
 * it running)  -- if not return NULL
 */
qthread_t INTERNAL *qt_threadqueue_dequeue_specific(qt_threadqueue_t *q,
                                                    void *value) {
  qt_threadqueue_node_t *node = NULL;
  qthread_t *t = NULL;

  assert(q != NULL);

  QTHREAD_TRYLOCK_LOCK(&q->qlock);
  if (atomic_load_explicit(&q->qlength, memory_order_relaxed) > 0) {
    node = (qt_threadqueue_node_t *)q->tail;
    t = (node) ? (qthread_t *)node->value : NULL;
    while ((t != NULL) && (t->ret != value)) {
      node = (qt_threadqueue_node_t *)node->prev;
      t = (node) ? (qthread_t *)node->value : NULL;
    }
    if ((node != NULL)) {
      if (node != q->tail) {
        if (node == q->head) {
          q->head = node->next; // reset front ptr
        } else {
          node->prev->next = node->next;
        }
        node->next->prev = node->prev; // reset back ptr (know we're not tail
        node->next = NULL;
        node->prev = q->tail;
        q->tail->next = node;
        q->tail = node;
      }
    }
  }
  QTHREAD_TRYLOCK_UNLOCK(&q->qlock);

  return (t);
}

void INTERNAL qthread_steal_enable(void) { steal_disable = 0; }

void INTERNAL qthread_steal_disable(void) { steal_disable = 1; }

qthread_shepherd_id_t INTERNAL
qt_threadqueue_choose_dest(qthread_shepherd_t *curr_shep) {
  if (curr_shep) {
    return curr_shep->shepherd_id;
  } else {
    return (qthread_shepherd_id_t)0;
  }
}

size_t INTERNAL qt_threadqueue_policy(const enum threadqueue_policy policy) {
  switch (policy) {
    default: return THREADQUEUE_POLICY_UNSUPPORTED;
  }
}

/* vim:set expandtab: */
