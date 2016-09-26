/* 
 * myth_spinlock_func.h
 */

#pragma once
#ifndef MYTH_SPINLOCK_FUNC_H_
#define MYTH_SPINLOCK_FUNC_H_

#include "myth/myth_spinlock.h"
#include "myth_mem_barrier_func.h"
#include "myth_config.h"

/* ----------- spin locks----------- */

static inline int myth_spin_init_body(myth_spinlock_t *lock) {
  lock->locked = 0;
  return 0;
}

static inline int myth_spin_destroy_body(myth_spinlock_t *lock) {
  (void)lock;
  return 0;
}

static inline int myth_spin_trylock_body(myth_spinlock_t *lock);

static inline int myth_spin_lock_body(myth_spinlock_t *lock) {
  while (!myth_spin_trylock_body(lock)) {
  }
  return 0;
}

static inline int myth_compare_and_set_int(volatile int * a, int oldv, int newv) {
  return __sync_bool_compare_and_swap(a, oldv, newv);
}

static inline int myth_spin_trylock_body(myth_spinlock_t *lock) {
  if (myth_compare_and_set_int(&lock->locked, 0, 1)) {
    myth_rwbarrier();
    return 1;
  } else {
    return 0;
  }
}

static inline int myth_spin_unlock_body(myth_spinlock_t *lock) {
  myth_rwbarrier();
  lock->locked = 0;
  return 0;
}

#endif /* MYTH_SPINLOCK_FUNC_H_ */
