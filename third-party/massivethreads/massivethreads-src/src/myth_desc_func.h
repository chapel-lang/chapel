/* 
 * myth_desc.h
 */
#pragma once
#ifndef MYTH_DESC_H_
#define MYTH_DESC_H_

#include <stdio.h>
#include <stddef.h>
#include <sys/mman.h>

#include "myth/myth.h"
#include "myth_config.h"
#include "myth_worker.h"
#include "myth_thread.h"

static inline myth_thread_t myth_context_to_thread(myth_running_env_t env,myth_context_t ctx)
{
  if (ctx==&env->sched.context){return NULL;}//scheduler
  char *ctx_ptr=(char*)ctx;
  return (myth_thread_t)(&ctx_ptr[-offsetof(struct myth_thread,context)]);
}

static inline void myth_desc_join_set(myth_thread_t thread,myth_thread_t wait_thread)
{
  thread->join_thread=wait_thread;
}

static inline void myth_desc_set_not_runnable(myth_thread_t thread)
{
  thread->status=MYTH_STATUS_BLOCKED;
}

static inline int myth_desc_is_runnable(myth_thread_t thread)
{
  return thread->status==MYTH_STATUS_READY;
}

static inline int myth_desc_is_finished(myth_thread_t thread)
{
  return thread->status>=MYTH_STATUS_FREE_READY;
}

static inline void myth_desc_set_detached(myth_thread_t th)
{
  th->detached=1;
}

#endif /* MYTH_DESC_H_ */
