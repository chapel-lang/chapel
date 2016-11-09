/* 
 * myth_spinlock.h
 */
#pragma once
#ifndef MYTH_SPINLOCK_H
#define MYTH_SPINLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

  /* 
     Type: myth_spinlock_t
   */
  typedef struct {
    volatile int locked;
  } myth_spinlock_t;
#define MYTH_SPINLOCK_INITIALIZER { 0 }

  /* 
     Function: myth_spin_init
   */
  int myth_spin_init(myth_spinlock_t *lock);

  /* 
     Function: myth_spin_destroy
   */
  int myth_spin_destroy(myth_spinlock_t *lock);

  /* 
     Function: myth_spin_lock
   */
  int myth_spin_lock(myth_spinlock_t *lock);

  /* 
     Function: myth_spin_trylock
   */
  int myth_spin_trylock(myth_spinlock_t *lock);

  /* 
     Function: myth_spin_unlock
   */
  int myth_spin_unlock(myth_spinlock_t *lock);


#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* MYTH_SPINLOCK_H */
