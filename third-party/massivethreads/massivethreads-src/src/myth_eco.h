/* 
 * myth_eco.h
 */
#pragma once
#ifndef _MYTH_ECO_H_
#define _MYTH_ECO_H_

#include <linux/futex.h>
#include <limits.h>
#include <semaphore.h>

#include "myth_config.h"
#include "myth_worker.h"

/* TODO: probably merge them into one  */
#define MYTH_ECO_DEBUG 0
#define MYTH_ECO_MODE_DEBUG 0

typedef struct sleep_queue {
  struct sleep_queue *next;
  struct sleep_queue *tail;
  int *head_sem;
  int head_rank;
}sleep_queue,*sleep_queue_t;


extern sleep_queue_t g_sleep_queue;
extern pthread_mutex_t *queue_lock;
extern int sleeper;
extern int task_num;
extern struct myth_running_env *g_envs;
extern int g_eco_mode_enabled;

#if MYTH_ECO_MODE
static void myth_eco_sched_loop(myth_running_env_t env);
#endif
myth_thread_t myth_eco_steal(int rank);
myth_thread_t myth_eco_all_task_check(myth_running_env_t env);

void myth_sleep_1(void);
void myth_sleep_2(int num);
void myth_go_asleep(void);
int myth_wakeup_one(void);
void myth_wakeup_all(void);
void myth_wakeup_all_force(void);
void myth_eco_init(void);
void myth_eco_des(void);

int myth_sleeper_push(int *sem, int rank, int num);
sleep_queue_t myth_sleeper_pop(void);
int futex_wait( void *futex, int comparand );
int futex_wakeup_one( void *futex );
int futex_wakeup_n( void *futex, int n );
int futex_wakeup_all( void *futex );
int fetch_and_store(volatile void *ptr, int addend);

#endif /* _MYTH_ECO_H_ */
