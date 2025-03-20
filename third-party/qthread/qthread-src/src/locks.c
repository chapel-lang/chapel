#include <assert.h>
#include <stdbool.h>

/* The API */
#include "qthread/qthread.h"
#include <qthread/hash.h>

/* Internal Headers */
#include "qt_alloc.h"
#include "qt_atomics.h"
#include "qt_feb.h"
#include "qt_hash.h"
#include "qt_locks.h"
#include "qt_visibility.h"

#define SPINLOCK_IS_RECURSIVE (-1)
#define SPINLOCK_IS_NOT_RECURSIVE (-2)

#define QTHREAD_CHOOSE_STRIPE2(addr)                                           \
  (qt_hash64((uint64_t)(uintptr_t)addr) & (QTHREAD_LOCKING_STRIPES - 1))
#define LOCKBIN(key) QTHREAD_CHOOSE_STRIPE2(key)
extern unsigned int QTHREAD_LOCKING_STRIPES;

qt_hash *qthread_spinlock_buckets;

static void qthread_spinlock_destroy_fn(qthread_spinlock_t *l) {
  QTHREAD_TRYLOCK_DESTROY_PTR(&l->lock);
}

static qthread_spinlock_t *lock_hashmap_get(aligned_t const *key) {
  if (!qthread_spinlock_buckets) return NULL;

  return qt_hash_get(qthread_spinlock_buckets[LOCKBIN(key)], key);
}

static int lock_hashmap_put(aligned_t const *key, qthread_spinlock_t *val) {
  if (!qthread_spinlock_buckets) return QTHREAD_OPFAIL;

  if (PUT_SUCCESS ==
      qt_hash_put(qthread_spinlock_buckets[LOCKBIN(key)], key, val))
    return QTHREAD_SUCCESS;

  return QTHREAD_OPFAIL;
}

static int lock_hashmap_remove(aligned_t const *key) {
  if (!qthread_spinlock_buckets) return QTHREAD_OPFAIL;
  void *val = NULL;
  if (qt_hash_pop(&val, qthread_spinlock_buckets[LOCKBIN(key)], key)) {
    if (val) qt_free(val);
    return QTHREAD_SUCCESS;
  }

  return QTHREAD_OPFAIL;
}

static bool is_spin_lock_hashed(aligned_t const *a) {
  return (NULL != lock_hashmap_get(a));
}

INTERNAL int spinlocks_initialize(void) {
  qthread_spinlock_buckets = NULL;
  return QTHREAD_SUCCESS;
}

INTERNAL int spinlocks_finalize(void) {
  if (qthread_spinlock_buckets) {
    for (unsigned i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
      assert(qthread_spinlock_buckets[i]);
      qt_hash_destroy_deallocate(
        qthread_spinlock_buckets[i],
        (qt_hash_deallocator_fn)qthread_spinlock_destroy_fn);
    }
    qt_free(qthread_spinlock_buckets);
  }
  return QTHREAD_SUCCESS;
}

/* locks over addresses using internal hashmaps */

static int spinlock_init_hashed(aligned_t const *a, bool const is_recursive) {
  uint_fast8_t need_sync = 1;

  if (!qthread_spinlock_buckets) {
    qthread_spinlock_buckets =
      (qt_hash *)qt_malloc(sizeof(qt_hash) * QTHREAD_LOCKING_STRIPES);
    assert(qthread_spinlock_buckets);
    for (unsigned i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
      qthread_spinlock_buckets[i] = qt_hash_create(need_sync);
      assert(qthread_spinlock_buckets[i]);
    }
  }

  if (!is_spin_lock_hashed(a)) {
    qthread_spinlock_t *l = qt_malloc(sizeof(qthread_spinlock_t));
    assert(l);
    l->state.s =
      is_recursive ? SPINLOCK_IS_RECURSIVE : SPINLOCK_IS_NOT_RECURSIVE;
    l->state.count = 0;
    QTHREAD_TRYLOCK_INIT_PTR(&l->lock);
    lock_hashmap_put(a, l);
    return QTHREAD_SUCCESS;
  }
  return QTHREAD_OPFAIL;
}

static int spinlock_destroy_hashed(aligned_t const *a) {
  return lock_hashmap_remove(a);
}

static int spinlock_lock_hashed(aligned_t const *a) {
  qthread_spinlock_t *l = lock_hashmap_get(a);
  if (l != NULL) {
    if (l->state.s >= SPINLOCK_IS_RECURSIVE) {
      if (l->state.s == qthread_readstate(CURRENT_UNIQUE_WORKER)) { // Reentrant
        ++l->state.count;
        MACHINE_FENCE;
      } else {
        QTHREAD_TRYLOCK_LOCK(&l->lock);
        l->state.s = qthread_readstate(CURRENT_UNIQUE_WORKER);
        ++l->state.count;
        MACHINE_FENCE;
      }
    } else {
      QTHREAD_TRYLOCK_LOCK(&l->lock);
    }
    return QTHREAD_SUCCESS;
  }
  return QTHREAD_OPFAIL;
}

static int spinlock_trylock_hashed(aligned_t const *a) {
  qthread_spinlock_t *l = lock_hashmap_get(a);
  if (l != NULL) {
    if (l->state.s >= SPINLOCK_IS_RECURSIVE) {
      if (l->state.s == qthread_readstate(CURRENT_UNIQUE_WORKER)) { // Reentrant
        ++l->state.count;
        MACHINE_FENCE;
      } else {
        if (QTHREAD_TRYLOCK_TRY(&l->lock)) {
          l->state.s = qthread_readstate(CURRENT_UNIQUE_WORKER);
          ++l->state.count;
          MACHINE_FENCE;
        } else {
          return QTHREAD_OPFAIL;
        }
      }
    } else {
      return QTHREAD_TRYLOCK_TRY(&l->lock);
    }
    return QTHREAD_SUCCESS;
  }
  return QTHREAD_OPFAIL;
}

static int spinlock_unlock_hashed(aligned_t const *a) {
  qthread_spinlock_t *l = lock_hashmap_get(a);
  if (l != NULL) {
    if (l->state.s >= SPINLOCK_IS_RECURSIVE) {
      if (l->state.s == qthread_readstate(CURRENT_UNIQUE_WORKER)) {
        --l->state.count;
        if (!l->state.count) {
          l->state.s = SPINLOCK_IS_RECURSIVE; // Reset
          MACHINE_FENCE;
          QTHREAD_TRYLOCK_UNLOCK(&l->lock);
        }
      } else {
        if (l->state.count) return QTHREAD_OPFAIL;
      }
    } else {
      QTHREAD_TRYLOCK_UNLOCK(&l->lock);
    }
    return QTHREAD_SUCCESS;
  }
  return QTHREAD_OPFAIL;
}

/* locks over lock types externally allocated */

static int spinlock_init(qthread_spinlock_t *a, bool const is_recursive) {
  if (is_recursive) {
    qthread_spinlock_t const init_mutex = QTHREAD_RECURSIVE_MUTEX_INITIALIZER;
    memcpy(a, &init_mutex, sizeof(qthread_spinlock_t));
  } else {
    qthread_spinlock_t const init_mutex = QTHREAD_MUTEX_INITIALIZER;
    memcpy(a, &init_mutex, sizeof(qthread_spinlock_t));
  }
  return QTHREAD_SUCCESS;
}

static int spinlock_destroy(qthread_spinlock_t *a) { return QTHREAD_SUCCESS; }

static int spinlock_lock(qthread_spinlock_t *a) {
  qthread_spinlock_t *l = a;
  if (l != NULL) {
    if (l->state.s >= SPINLOCK_IS_RECURSIVE) {
      if (l->state.s == qthread_readstate(CURRENT_UNIQUE_WORKER)) { // Reentrant
        ++l->state.count;
        MACHINE_FENCE;
      } else {
        QTHREAD_TRYLOCK_LOCK(&l->lock);
        l->state.s = qthread_readstate(CURRENT_UNIQUE_WORKER);
        ++l->state.count;
        MACHINE_FENCE;
      }
    } else {
      QTHREAD_TRYLOCK_LOCK(&l->lock);
    }
    return QTHREAD_SUCCESS;
  }
  return QTHREAD_OPFAIL;
}

static int spinlock_trylock(qthread_spinlock_t *a) {
  qthread_spinlock_t *l = a;
  if (l != NULL) {
    if (l->state.s >= SPINLOCK_IS_RECURSIVE) {
      if (l->state.s == qthread_readstate(CURRENT_UNIQUE_WORKER)) { // Reentrant
        ++l->state.count;
        MACHINE_FENCE;
      } else {
        if (QTHREAD_TRYLOCK_TRY(&l->lock)) {
          l->state.s = qthread_readstate(CURRENT_UNIQUE_WORKER);
          ++l->state.count;
          MACHINE_FENCE;
        } else {
          return QTHREAD_OPFAIL;
        }
      }
    } else {
      return QTHREAD_TRYLOCK_TRY(&l->lock);
    }
    return QTHREAD_SUCCESS;
  }
  return QTHREAD_OPFAIL;
}

static int spinlock_unlock(qthread_spinlock_t *a) {
  qthread_spinlock_t *l = a;
  if (l != NULL) {
    if (l->state.s >= SPINLOCK_IS_RECURSIVE) {
      if (l->state.s == qthread_readstate(CURRENT_UNIQUE_WORKER)) {
        --l->state.count;
        if (!l->state.count) {
          l->state.s = SPINLOCK_IS_RECURSIVE; // Reset
          MACHINE_FENCE;
          QTHREAD_TRYLOCK_UNLOCK(&l->lock);
        }
      } else {
        if (l->state.count) return QTHREAD_OPFAIL;
      }
    } else {
      QTHREAD_TRYLOCK_UNLOCK(&l->lock);
    }
    return QTHREAD_SUCCESS;
  }
  return QTHREAD_OPFAIL;
}

/* Functions to implement FEB-ish locking/unlocking*/

int API_FUNC qthread_lock_init(aligned_t const *a, bool const is_recursive) {
  return spinlock_init_hashed(a, is_recursive);
}

int API_FUNC qthread_lock_destroy(aligned_t *a) {
  if (!is_spin_lock_hashed(a)) { return QTHREAD_SUCCESS; }
  return spinlock_destroy_hashed(a);
}

int API_FUNC qthread_lock(aligned_t const *a) {
  if (!is_spin_lock_hashed(a)) { return qthread_readFE(NULL, a); }
  return spinlock_lock_hashed(a);
}

int API_FUNC qthread_trylock(aligned_t const *a) {
  if (!is_spin_lock_hashed(a)) { return qthread_readFE_nb(NULL, a); }
  return spinlock_trylock_hashed(a);
}

int API_FUNC qthread_unlock(aligned_t const *a) {
  if (!is_spin_lock_hashed(a)) { return qthread_fill(a); }
  return spinlock_unlock_hashed(a);
}

int API_FUNC qthread_spinlock_init(qthread_spinlock_t *a,
                                   bool const is_recursive) {
  return spinlock_init(a, is_recursive);
}

int API_FUNC qthread_spinlock_destroy(qthread_spinlock_t *a) {
  return spinlock_destroy(a);
}

int API_FUNC qthread_spinlock_lock(qthread_spinlock_t *a) {
  return spinlock_lock(a);
}

int API_FUNC qthread_spinlock_trylock(qthread_spinlock_t *a) {
  return spinlock_trylock(a);
}

int API_FUNC qthread_spinlock_unlock(qthread_spinlock_t *a) {
  return spinlock_unlock(a);
}

#undef qt_hash_t

/* vim:set expandtab: */
