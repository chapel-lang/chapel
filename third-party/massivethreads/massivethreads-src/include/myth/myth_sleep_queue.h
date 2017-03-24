/* 
 * myth_sleep_queue.h
 */
#pragma once
#ifndef MYTH_SLEEP_QUEUE_H_
#define MYTH_SLEEP_QUEUE_H_

#include "myth_spinlock.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MYTH_SLEEP_QUEUE_DBG
#define MYTH_SLEEP_QUEUE_DBG 0
#endif

  /* myth_sleep_queue_item_t represents a pointer 
     to any data structure that has "next" field
     in its head */
  typedef struct myth_sleep_queue_item {
    struct myth_sleep_queue_item * next;
  } myth_sleep_queue_item, * myth_sleep_queue_item_t;

  typedef struct {
    volatile myth_sleep_queue_item_t head;
    volatile myth_sleep_queue_item_t tail;
    myth_spinlock_t ilock[1];
  } myth_sleep_queue_t;
  
#define MYTH_SLEEP_QUEUE_INITIALIZER { 0, 0, MYTH_INTERNAL_LOCK_INITIALIZER }

  typedef struct {
    volatile myth_sleep_queue_item_t top;
  } myth_sleep_stack_t;

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* MYTH_SLEEP_QUEUE_H_ */
