/* 
 * myth_sched_func.h
 */
#pragma once
#ifndef MYTH_SCHED_FUNC_H_
#define MYTH_SCHED_FUNC_H_
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sched.h>

//Body of basic functions

#include "myth/myth.h"

#include "myth_config.h"
#include "myth_context.h"
#include "myth_sched.h"
#include "myth_worker.h"
#include "myth_thread.h"
#include "myth_misc.h"
#include "myth_tls.h"

#if MYTH_ECO_MODE
#include "myth_eco.h"
#endif

#include "myth_context_func.h"
#include "myth_worker_func.h"
#include "myth_misc_func.h"
#include "myth_spinlock_func.h"
#include "myth_desc_func.h"
#include "myth_tls_func.h"

#ifndef PAGE_ALIGN
#define PAGE_ALIGN(n) ((((n)+(PAGE_SIZE)-1)/(PAGE_SIZE))*PAGE_SIZE)
#endif

static inline size_t myth_align_stack_size(size_t newsize){
  return PAGE_ALIGN(newsize);
}

#define myth_dprintf(...) myth_dprintf_1((char*)__func__,__VA_ARGS__)
void myth_dprintf_1(char *func,char *fmt,...);

//Return a new thread descriptor
static inline myth_thread_t get_new_myth_thread_struct_desc(myth_running_env_t env) {
#if MYTH_SPLIT_STACK_DESC
  myth_thread_t ret;
#if MYTH_ALLOC_PROF
  env->prof_data.dalloc_cnt++;
#endif
  void * v_ret = myth_freelist_pop(&env->freelist_desc);
  if (v_ret){
    return v_ret;
  } else {
    //Allocate
    int i;
    size_t th_size = sizeof(struct myth_thread);
    size_t alloc_size = th_size * STACK_ALLOC_UNIT;
#if MYTH_ALLOC_PROF
    env->prof_data.dmalloc_cnt++;
    uint64_t t0 = myth_get_rdtsc();
#endif
#if ALLOCATE_STACK_BY_MALLOC
    void * th_ptr = myth_flmalloc(env->rank, alloc_size);
#else
    alloc_size += 0xFFF;
    alloc_size &= ~0xFFF;
#if defined(MAP_STACK)
    char * th_ptr = myth_mmap(NULL,alloc_size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK,-1,0);
#else
    char * th_ptr = myth_mmap(NULL,alloc_size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
#endif
#endif
#if MYTH_ALLOC_PROF
    uint64_t t1 = myth_get_rdtsc();
    env->prof_data.dmalloc_cycles += t1 - t0;
    uint64_t t2 = myth_get_rdtsc();
#endif
    for (i = 0; i < STACK_ALLOC_UNIT; i++){
      ret = (myth_thread_t)th_ptr;
#if MYTH_DESC_REUSE_CHECK
      myth_spin_init_body(&ret->sanity_check);
#endif
      myth_spin_init_body(&ret->lock);
      if (i < STACK_ALLOC_UNIT - 1){
	myth_freelist_push(&env->freelist_desc, ret);
	th_ptr += th_size;
      }
    }
#if MYTH_ALLOC_PROF
    uint64_t t3 = myth_get_rdtsc();
    env->prof_data.daddlist_cycles += t3 - t2;
#endif
  }
  return ret;
#else
  myth_thread_t ret;
  //th=NULL;
  //Search freelist index
#if MYTH_ALLOC_PROF
  env->prof_data.alloc_cnt++;
#endif
  void * v_ret = myth_freelist_pop(&env->freelist_ds);
  if (v_ret){
    return v_ret;
  } else {
    //Allocate
    int i;
    size_t st_size = MYTH_MALLOC_SIZE_TO_RSIZE(REAL_STACK_SIZE);
    size_t th_size = st_size + sizeof(myth_thread);
    size_t alloc_size = th_size * STACK_ALLOC_UNIT;
#if MYTH_ALLOC_PROF
    env->prof_data.malloc_cnt++;
    uint64_t t0 = myth_get_rdtsc();
#endif
#if ALLOCATE_STACK_BY_MALLOC
    void * th_ptr = myth_flmalloc(env->rank, alloc_size);
#else
    alloc_size += 0xFFF;
    alloc_size &= ~0xFFF;
#if defined(MAP_STACK)
    void * th_ptr = mmap(NULL,alloc_size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK,-1,0);
#else
    void * th_ptr = mmap(NULL,alloc_size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
#endif
#endif
#if MYTH_ALLOC_PROF
    uint64_t t1 = myth_get_rdtsc();
    env->prof_data.malloc_cycles += t1 - t0;
    uint64_t t2 = myth_get_rdtsc();
#endif
    for (i=0; i<STACK_ALLOC_UNIT; i++){
      ret = (myth_thread_t)(th_ptr+st_size);
      ret->stack = (th_ptr + st_size - sizeof(void*));
      ret->stack_size = st_size;
      myth_spin_init_body(&ret->lock);
      if (i < STACK_ALLOC_UNIT - 1){
	ret = myth_freelist_push(&env->freelist_ds);
	th_ptr += th_size;
      }
    }
#if MYTH_ALLOC_PROF
    uint64_t t3 = myth_get_rdtsc();
    env->prof_data.addlist_cycles += t3 - t2;
#endif
  }
  return ret;
#endif
}


// fast when size_in_bytes == 0

// Return a new thread descriptor
static inline void * get_new_myth_thread_struct_stack(myth_running_env_t env,
						      size_t size_in_bytes) {
#if MYTH_SPLIT_STACK_DESC
#if MYTH_ALLOC_PROF
  env->prof_data.salloc_cnt ++;
#endif /* MYTH_ALLOC_PROF */
  if (size_in_bytes) {
    /* let's say sizeof(void*) == 8, original size_in_bytes is tiny,
       then size_in_bytes is rounded up to 4096.
       for brevity let's say the bottom address is zero.

          4088-4095: size_in_bytes
th_ptr -> 4080-4087: 
            ...
     */

    
    //Round up to 4KB
    size_in_bytes +=  0xFFF;
    size_in_bytes &= ~0xFFF;
    char * th_ptr = myth_flmalloc(env->rank, size_in_bytes);
    th_ptr += size_in_bytes - (sizeof(void*) * 2);
    uintptr_t *blk_size = (uintptr_t*) (th_ptr + sizeof(void*));
    *blk_size = size_in_bytes;
    return th_ptr;
  }
  void * ret = myth_freelist_pop(&env->freelist_stack);
  if (ret) {
    return ret;
  } else {
    //Allocate
    int i;
    size_t th_size = g_attr.stacksize;
    
#if USE_STACK_GUARDPAGE
    th_size += PAGE_SIZE;
#endif /* USE_STACK_GUARDPAGE */
    size_t alloc_size = th_size * STACK_ALLOC_UNIT;
#if MYTH_ALLOC_PROF
    env->prof_data.smalloc_cnt ++;
    uint64_t t0 = myth_get_rdtsc();
#endif /* MYTH_ALLOC_PROF */

#if ALLOCATE_STACK_BY_MALLOC
    void * th_ptr = myth_flmalloc(env->rank, alloc_size);
#else
    alloc_size += 0xFFF;
    alloc_size &= ~0xFFF;
#if defined(MAP_STACK)
    char * th_ptr = myth_mmap(NULL, alloc_size, PROT_READ|PROT_WRITE, 
			      MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK, -1, 0);
#else
    char * th_ptr = myth_mmap(NULL, alloc_size, PROT_READ|PROT_WRITE,
			      MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
#endif /* MAP_STACK */
#endif /* ALLOCATE_STACK_BY_MALLOC */

#if MYTH_ALLOC_PROF
    uint64_t t1 = myth_get_rdtsc();
    env->prof_data.smalloc_cycles += t1 - t0;
    uint64_t t2 = myth_get_rdtsc();
#endif /* MYTH_ALLOC_PROF */
#if USE_STACK_GUARDPAGE
    char *pr_ptr = th_ptr;
    for (i = 0; i < STACK_ALLOC_UNIT; i++) {
      mprotect(pr_ptr, PAGE_SIZE, PROT_NONE);
      pr_ptr += th_size;
    }
#endif /* USE_STACK_GUARDPAGE */
    th_ptr += th_size - (sizeof(void*) * 2);
    for (i = 0; i < STACK_ALLOC_UNIT; i++){
      ret = (void**) th_ptr;
      uintptr_t *blk_size = (uintptr_t*)(th_ptr + sizeof(void*));
      *blk_size = 0;	  //indicates default
      if (i < STACK_ALLOC_UNIT - 1) {
	myth_freelist_push(&env->freelist_stack, ret);
	th_ptr += th_size;
      }
    }
#if MYTH_ALLOC_PROF
    uint64_t t3 = myth_get_rdtsc();
    env->prof_data.saddlist_cycles += t3 - t2;
#endif /* MYTH_ALLOC_PROF */
  }
  return ret;
#else
  return NULL;
#endif /* MYTH_SPLIT_STACK_DESC */
}

static inline void init_myth_thread_struct(myth_running_env_t env, myth_thread_t th) {
  th->status = MYTH_STATUS_READY;
  th->join_thread = NULL;
  th->detached = 0;
  th->cancel_enabled = 1;
  th->cancelled = 0;
  //th->pickle_ptr=NULL;
  th->env = env;
#if MYTH_ENABLE_THREAD_ANNOTATION && MYTH_COLLECT_LOG
  sprintf(th->annotation_str, "%p@%d", (void*)th, th->recycle_count);
  th->recycle_count ++;
#endif
#if MYTH_DESC_REUSE_CHECK
  assert(myth_spin_trylock_body(&th->sanity_check));
#endif
#if 0
  myth_queue_init_thread_data(&th->queue_data);
#endif
}

//Release thread descriptor
static inline void free_myth_thread_struct_desc(myth_running_env_t e,myth_thread_t th)
{
  /*if (e!=myth_get_current_env()){
    fprintf(stderr,"Rank error %d->%d\n",myth_get_current_env()->rank,e->rank);
    assert(0);
    }*/
#if MYTH_SPLIT_STACK_DESC
  myth_assert(th);
#if 0
  myth_queue_fini_thread_data(&th->queue_data);
#endif
#if FREE_MYTH_THREAD_STRUCT_DEBUG
  myth_dprintf("thread descriptor %p is freed\n",th);
#endif
#if MYTH_DESC_REUSE_CHECK
  myth_spin_unlock_body(&th->sanity_check);
#endif
  //Add to a freelist
  myth_freelist_push(&e->freelist_desc,(void*)th);
#else
  myth_assert(th);
#if FREE_MYTH_THREAD_STRUCT_DEBUG
  myth_dprintf("thread stack %p is freed\n",th);
#endif
  //Add to a freelist
  myth_freelist_push(e->freelist_ds,(void*)th);
#endif
}

//Release thread descriptor
static inline void free_myth_thread_struct_stack(myth_running_env_t e,myth_thread_t th)
{
  /*if (e!=myth_get_current_env()){
    fprintf(stderr,"Rank error %d->%d\n",myth_get_current_env()->rank,e->rank);
    assert(0);
    }*/
#if MYTH_SPLIT_STACK_DESC
  void **ptr;
  myth_assert(th);
#if FREE_MYTH_THREAD_STRUCT_DEBUG
  myth_dprintf("thread stack %p is freed\n",th);
#endif
  if (th->stack) {
    //Add to a freelist
    ptr = (void**)th->stack;

    uintptr_t *blk_size = (uintptr_t*)(((uint8_t*)ptr) + sizeof(void*));
    if (*blk_size == 0) {
      myth_freelist_push(&e->freelist_stack, ptr);
    } else {
      void *stack_start=(((uint8_t*)ptr)-(*blk_size)+(sizeof(void*)*2));
      myth_flfree(e->rank,(size_t)(*blk_size),stack_start);
    }
  }
#endif
}

//Return a new thread descriptor from external
static inline myth_thread_t get_new_myth_thread_struct_desc_ext(void) {
  return myth_malloc(sizeof(struct myth_thread));
}

//Release thread descriptor
static inline void free_myth_thread_struct_desc_ext(myth_thread_t th) {
  myth_free(th);
}

MYTH_CTX_CALLBACK void myth_create_1(void *arg1,void *arg2,void *arg3) {
  MAY_BE_UNUSED uint64_t t0,t1;
  myth_running_env_t env = arg1;
  myth_thread_t new_thread = arg3;
  myth_thread_t this_thread = env->this_thread;
  myth_func_t fn = (myth_func_t)arg2;
  t0 = 0; t1 = 0;
#if MYTH_CREATE_PROF_DETAIL
  t1 = myth_get_rdtsc();
  env->prof_data.create_switch += t1 - env->prof_data.create_d_tmp;
  t0 = myth_get_rdtsc();
#endif

  //Push current thread to runqueue
  myth_queue_push(&env->runnable_q, this_thread);

#if MYTH_CREATE_PROF_DETAIL
  t1 = myth_get_rdtsc();
  env->prof_data.create_push += t1 - t0;
  env->prof_data.create_d_cnt++;
#endif
  env->this_thread = new_thread;
#if MYTH_CREATE_PROF
  t1=myth_get_rdtsc();
  env->prof_data.create_cycles+=t1-env->prof_data.create_cycles_tmp;
  env->prof_data.create_cnt++;
#endif
  //myth_log_add(env,MYTH_LOG_USER);
#if MYTH_ENTRY_POINT_DEBUG
  myth_dprintf("Running thread %p(arg:%p)\n",new_thread,new_thread->arg);
#endif
#if MYTH_ECO_MODE
  if (g_eco_mode_enabled){
    if (sleeper > 0) {
      myth_wakeup_one();
    }
  }
#endif
  // Call entry point function
  new_thread->result = (*fn)(new_thread->result);
  //myth_log_add(new_thread->env,MYTH_LOG_INT);
  myth_entry_point_cleanup(new_thread);
}

/* --------------------------------------------------
   body of public functions
   -------------------------------------------------- */


/* --------
   create
   -------- */

static inline int myth_create_ex_body(myth_thread_t * id,
				      myth_thread_attr_t * attr,
				      myth_func_t func, void *arg) {
#if MYTH_CREATE_PROF
  uint64_t t0 = myth_get_rdtsc();
#endif /* MYTH_CREATE_PROF */
#if MYTH_CREATE_PROF_DETAIL
  uint64_t t0 = myth_get_rdtsc();
#endif /* MYTH_CREATE_PROF_DETAIL */

  int _ = myth_ensure_init();
  size_t stack_size       = (attr ? attr->stacksize : 0);
  size_t custom_data_size = (attr ? attr->custom_data_size : 0);
  void * custom_data      = (attr ? attr->custom_data : 0);
  int child_first         = (attr ? attr->child_first : 1);
  myth_running_env_t env = myth_get_current_env();
  //myth_log_add(env,MYTH_LOG_INT);
  // Allocate new thread descriptor
  myth_thread_t new_thread = get_new_myth_thread_struct_desc(env);
  (void)_;
  new_thread->next = 0;
#if MYTH_DEBUG_JOIN_FCC
  new_thread->join_called_at = 0;
  new_thread->child_status_when_join_was_called = "";
  new_thread->finished_at = 0;
  new_thread->waiter = (struct myth_thread *)-1;
  new_thread->when_I_finished = "";
#endif
  myth_tls_tree_init(new_thread->tls);

#if MYTH_SPLIT_STACK_DESC /* default */
  // allocate stack and get pointer
  void * stk = get_new_myth_thread_struct_stack(env, stack_size);
  new_thread->stack = stk;
  new_thread->stack_size = stack_size;
#else
  void * stk = new_thread->stack;
#endif /* MYTH_SPLIT_STACK_DESC */

  //Allocate custom data region on stack
  if (custom_data_size > 0){
    new_thread->custom_data_size = custom_data_size;
    intptr_t i_stk = (intptr_t)stk;
    //Align 16byte
    i_stk -= 16 + (((custom_data_size + 15) >> 4) << 4);
    new_thread->custom_data_ptr = (void*)(i_stk + 16);
    memcpy((void*)(i_stk + 16), custom_data, custom_data_size);
    stk = (void*)i_stk;
  } else {
    new_thread->custom_data_size = 0;
  }

  // Initialize thread descriptor
  init_myth_thread_struct(env, new_thread);
  new_thread->result = arg;

  size_t stk_size = stack_size - sizeof(void*) * 2;
  if (child_first){
    myth_make_context_empty(&new_thread->context, stk, stk_size);

#if MYTH_CREATE_PROF_DETAIL
    t1 = myth_get_rdtsc();
    env->prof_data.create_alloc += t1-t0;
#endif /* MYTH_CREATE_PROF_DETAIL */

#if MYTH_CREATE_PROF
    env->prof_data.create_cycles_tmp = t0;
#endif /* MYTH_CREATE_PROF */

    myth_thread_t this_thread;
    // Push current thread to runqueue and switch context to new thread
    this_thread = env->this_thread;
#if MYTH_CREATE_PROF_DETAIL
    env->prof_data.create_d_tmp = myth_get_rdtsc();
#endif /* MYTH_CREATE_PROF_DETAIL */
    myth_swap_context_withcall(&this_thread->context,
			       &new_thread->context,
			       myth_create_1,
			       (void*)env, (void*)func, (void*)new_thread);
  } else {
    new_thread->entry_func = func;
    //Create context
    myth_make_context_voidcall(&new_thread->context, myth_entry_point,
			       stk, stk_size);

#if MYTH_CREATE_PROF_DETAIL
    t1 = myth_get_rdtsc();
    env->prof_data.create_alloc += t1-t0;
#endif /* MYTH_CREATE_PROF_DETAIL */

#if MYTH_CREATE_PROF
    env->prof_data.create_cycles_tmp=t0;
#endif /* MYTH_CREATE_PROF */

    //Push a new thread to runqueue
    myth_queue_push(&env->runnable_q, new_thread);
#if MYTH_CREATE_PROF
    t1 = myth_get_rdtsc();
    env->prof_data.create_cycles += t1 - t0;
    env->prof_data.create_cnt++;
#endif /* MYTH_CREATE_PROF */
  }
  id[0] = new_thread;
  return 0;
}

/* --------
   exit
   -------- */

static inline void myth_exit_body(void *ret) {
  myth_running_env_t env;
  myth_thread_t th;
  env = myth_get_current_env();
  th = env->this_thread;
  th->result = ret;
  myth_entry_point_cleanup(th);
}

/* --------
   join
   -------- */

static inline void myth_join_1(myth_running_env_t e,myth_thread_t th,void **result)
{
  if (result!=NULL){
    *result=th->result;
  }
  free_myth_thread_struct_desc(e,th);
}

MYTH_CTX_CALLBACK void myth_join_2(void *arg1,void *arg2,void *arg3)
{
  myth_running_env_t env=arg1;
  myth_thread_t th=arg2,next_thread=arg3;
  //Set join target
  myth_desc_join_set(th,env->this_thread);
  myth_spin_unlock_body(&th->lock);
  //Change current running thread
  env->this_thread=next_thread;
  //myth_log_add(env,MYTH_LOG_USER);
}

MYTH_CTX_CALLBACK void myth_join_3(void *arg1,void *arg2,void *arg3)
{
  myth_thread_t this_thread=arg1,th=arg2;
  (void)arg3;
  //Set join target
  myth_desc_join_set(th,this_thread);
  //Change current running thread
  myth_spin_unlock_body(&th->lock);
}

//Wait until the finish of a thread
static inline int myth_join_body(myth_thread_t th,void **result) {
  //TODO:Fix th->status is blocked after join
  MAY_BE_UNUSED uint64_t t0,t1;
  myth_running_env_t env;
  myth_thread_t this_thread;
  t0 = 0; t1 = 0;
#if MYTH_JOIN_PROF
  uint64_t t0, t1, t2, t3;
  t0 = myth_get_rdtsc();
#endif
#if MYTH_JOIN_PROF_DETAIL
  t0 = myth_get_rdtsc();
#endif
  env = myth_get_current_env();
  //myth_log_add(env,MYTH_LOG_INT);
  this_thread = env->this_thread;
#if MYTH_JOIN_DEBUG
  myth_dprintf("myth_join:join started\n");
#endif
#if QUICK_CHECK_ON_JOIN

#error "I told you to turn this off"
  
  //If target is finished, return immediately
  if (th->status == MYTH_STATUS_FREE_READY2){
#if MYTH_JOIN_DEBUG
    myth_dprintf("myth_join:join thread (%p) is already finished. Return immediately\n",th);
#endif
    //guarantee that successive read starts after this line
    myth_rbarrier();
#if MYTH_JOIN_PROF_DETAIL
    if (result) *result = th->result;
    t1 = myth_get_rdtsc();
    env->prof_data.join_join += t1 - t0;
    t0 = myth_get_rdtsc();
    free_myth_thread_struct_desc(env,th);
    t1 = myth_get_rdtsc();
    env->prof_data.join_release += t1 - t0;
    env->prof_data.join_d_cnt++;
#else
    myth_join_1(env,th,result);
#endif
#if MYTH_JOIN_PROF
    t1 = myth_get_rdtsc();
    env->prof_data.join_cycles += t1-t0;
    env->prof_data.join_cnt++;
#endif
    //myth_log_add(env,MYTH_LOG_USER);
    return 0;
  }
#endif
  //Obtain lock and check again
  myth_spin_lock_body(&th->lock);
  //If target is finished, return
  if (myth_desc_is_finished(th)){
#if MYTH_DEBUG_JOIN_FCC
    th->join_called_at = myth_get_rdtsc();
    th->child_status_when_join_was_called = "child has been finished";
#endif

#if MYTH_JOIN_DEBUG
    myth_dprintf("myth_join:join thread (%p) is already finished. Return immediately\n",th);
#endif
    myth_spin_unlock_body(&th->lock);
    while (th->status != MYTH_STATUS_FREE_READY2);
#if MYTH_JOIN_PROF_DETAIL
    if (result) *result = th->result;
    t1 = myth_get_rdtsc();
    env->prof_data.join_join += t1 - t0;
    t0 = myth_get_rdtsc();
    free_myth_thread_struct_desc(env,th);
    t1 = myth_get_rdtsc();
    env->prof_data.join_release += t1-t0;
    env->prof_data.join_d_cnt++;
#else
    myth_join_1(env,th,result);
#endif
#if MYTH_JOIN_PROF
    t1 = myth_get_rdtsc();
    env->prof_data.join_cycles += t1-t0;
    env->prof_data.join_cnt++;
#endif
    //myth_log_add(env,MYTH_LOG_USER);
    return 0;
  }
  //Set current thread as blocked
  myth_desc_set_not_runnable(this_thread);
#if MYTH_JOIN_DEBUG
  myth_dprintf("myth_join:%p is added to %p's waiting list\n",this_thread,th);
#endif
  //Get next runnable thread
  myth_thread_t next;
  next = myth_queue_pop(&env->runnable_q);
#if MYTH_JOIN_PROF
  t1 = myth_get_rdtsc();
#endif
  if (next){
#if MYTH_DEBUG_JOIN_FCC
    th->join_called_at = myth_get_rdtsc();
    th->child_status_when_join_was_called = "child not finished and go to next";
#endif
    next->env=env;
    //Switch to next runnable thread
    myth_swap_context_withcall(&this_thread->context,&next->context,myth_join_2,
			       (void*)env,(void*)th,(void*)next);
  }
  else{
#if MYTH_DEBUG_JOIN_FCC
    th->join_called_at = myth_get_rdtsc();
    th->child_status_when_join_was_called = "child not finished and go to sched";
#endif
    //myth_log_add(this_thread->env,MYTH_LOG_WS);
    //Since there is no runnable thread, switch to scheduler and do work-steaing
    myth_swap_context_withcall(&this_thread->context,&env->sched.context,myth_join_3,
			       (void*)this_thread,(void*)th,NULL);
  }
#if MYTH_JOIN_PROF
  t2 = myth_get_rdtsc();
#endif
  //myth_log_add(this_thread->env,MYTH_LOG_INT);
#if MYTH_JOIN_DEBUG
  myth_dprintf("myth_join:%p is resumed\n",this_thread);
#endif
#if 0
  myth_spin_lock_body(&th->lock);
  //In this position, target is guaranteed to be finished
  myth_assert(myth_desc_is_runnable(this_thread));
  myth_assert(myth_desc_is_finished(th));
  //Get return value
  myth_spin_unlock_body(&th->lock);
#endif
  while (th->status != MYTH_STATUS_FREE_READY2) { }
  myth_join_1(myth_get_current_env(),th,result);
#if MYTH_JOIN_PROF
  t3 = myth_get_rdtsc();
  env->prof_data.join_cycles += (t1 - t0) + (t3 - t2);
  env->prof_data.join_cnt++;
#endif
  //myth_log_add(this_thread->env,MYTH_LOG_USER);
  return 0;
}

/* --------
   tryjoin
   -------- */

/* --------
   timedjoin
   -------- */

/* --------
   create_join_various, create_join_many
   -------- */

typedef struct {
  void * ids;                   /* base pointer to strided array of myth_thread_t */
  void * attrs;			/* base pointer to strided array of myth_thread_attr_t */
  void * funcs;			/* base pointer to strided array of myth_func_t */
  void * args;			/* base pointer to strided array of args */
  void * results;		/* base pointer to strided array of results */
  size_t id_stride;		/* stride of ids   between consecutive threads */
  size_t attr_stride;		/* stride of attrs between consecutive threads */
  size_t func_stride;		/* stride of funcs between consecutive threads */
  size_t arg_stride;            /* stride of args  between consecutive threads */
  size_t result_stride;		/* stride of results between consecutive threads */
  long a;			/* from  */
  long b;			/* to */
} myth_create_join_various_arg;

static inline void * myth_create_join_various_ex_aux(void * meta_arg_) {
  myth_create_join_various_arg * meta_arg = meta_arg_;
  long a               = meta_arg->a;
  long b               = meta_arg->b;
  size_t id_stride     = meta_arg->id_stride;
  size_t attr_stride   = meta_arg->attr_stride;
  size_t func_stride   = meta_arg->func_stride;
  size_t arg_stride    = meta_arg->arg_stride;
  size_t result_stride = meta_arg->result_stride;

  if (b - a == 1) {
    void * ids     = (meta_arg->ids   ? (char *)meta_arg->ids   + a * id_stride : 0);
    void * funcs   = (char *)meta_arg->funcs   + a * func_stride;
    void * args    = (char *)meta_arg->args    + a * arg_stride; 
    void * results = (meta_arg->results ? (char *)meta_arg->results + a * result_stride : 0);  
    if (ids) {
      ((myth_thread_t *)ids)[0] = myth_self();
    }
    myth_func_t func = ((myth_func_t *)funcs)[0]; /* call f(x) */
    void * y = func(args);
    if (results) {
      ((void **)results)[0] = y;
    }
  } else {
    long c = (a + b) / 2;
    void * ids    = meta_arg->ids;
    void * attrs  = meta_arg->attrs;
    void * funcs  = meta_arg->funcs;
    void * args   = meta_arg->args;
    void * results = meta_arg->results;
    myth_create_join_various_arg carg[2] = { 
      { ids, attrs, funcs, args, results, 
	id_stride, attr_stride, func_stride, arg_stride, result_stride, 
	a, c },
      { ids, attrs, funcs, args, results, 
	id_stride, attr_stride, func_stride, arg_stride, result_stride, 
	c, b }
    };
    myth_thread_attr_t * attr_a = (myth_thread_attr_t *)(attrs ? (char *)attrs + a * attr_stride : 0);
    myth_thread_t cid = 0;
    int r0 = myth_create_ex_body(&cid, attr_a, myth_create_join_various_ex_aux, carg);
    assert(r0 == 0); /* TODO : better communicate error */
    void * r1 = myth_create_join_various_ex_aux(carg + 1);
    assert(r1 == 0); /* TODO : better communicate error */
    int r2 = myth_join_body(cid, 0);
    assert(r2 == 0); /* TODO : better communicate error */
  }
  return 0;
}

static inline int myth_create_join_various_ex_body(myth_thread_t * ids,
						   myth_thread_attr_t * attrs,
						   myth_func_t * funcs,
						   void * args,
						   void * results,
						   size_t id_stride,
						   size_t attr_stride,
						   size_t func_stride,
						   size_t arg_stride,
						   size_t result_stride,
						   long nthreads) {
  if (nthreads == 0) return 0;
  myth_create_join_various_arg arg[1] = {
    { 
      ids,       attrs,       funcs,       args,       results, 
      id_stride, attr_stride, func_stride, arg_stride, result_stride,
      0, nthreads 
    }
  };
  void * r = myth_create_join_various_ex_aux(arg);
  assert(r == 0);
  return 0;
}

static inline int myth_create_join_many_ex_body(myth_thread_t * ids,
						myth_thread_attr_t * attrs,
						myth_func_t func,
						void * args,
						void * results,
						size_t id_stride,
						size_t attr_stride,
						size_t arg_stride,
						size_t result_stride,
						long nthreads) {
  myth_func_t funcs[1] = { func };
  return myth_create_join_various_ex_body(ids, attrs, funcs, args, results,
					  id_stride, attr_stride, 0, arg_stride, result_stride,
					  nthreads);
}

/* --------
   detach
   -------- */

static inline int myth_detach_body(myth_thread_t th)
{
  if (th->status==MYTH_STATUS_FREE_READY2){
    //If a thread is finished, just release resource
    free_myth_thread_struct_desc(myth_get_current_env(),th);
    return 0;
  }
  //Obtain lock
  myth_spin_lock_body(&th->lock);
  if (myth_desc_is_finished(th)){//If a thread is finished, release resource
    myth_spin_unlock_body(&th->lock);
    while (th->status!=MYTH_STATUS_FREE_READY2);
    free_myth_thread_struct_desc(myth_get_current_env(),th);
  }
  else{//Set a thread as detached
    myth_desc_set_detached(th);
    myth_spin_unlock_body(&th->lock);
  }
  return 0;
}

/* --------
   attr_init
   -------- */

static inline int myth_thread_attr_init_body(myth_thread_attr_t * attr) {
  attr->stackaddr = 0;
  myth_globalattr_get_stacksize_body(0, &attr->stacksize);
  attr->detachstate = 0;
  myth_globalattr_get_guardsize_body(0, &attr->guardsize);
  myth_globalattr_get_child_first_body(0, &attr->child_first);
  return 0;
}

static inline int myth_thread_attr_getdetachstate_body(const myth_thread_attr_t *attr,
					 int *detachstate) {
  *detachstate = attr->detachstate;
  return 0;
}

static inline int myth_thread_attr_setdetachstate_body(myth_thread_attr_t *attr,
					 int detachstate) {
  attr->detachstate = detachstate;
  return 0;
}

static inline int myth_thread_attr_getguardsize_body(const myth_thread_attr_t *attr,
				       size_t *guardsize) {
  *guardsize = attr->guardsize;
  return 0;
}

static inline int myth_thread_attr_setguardsize_body(myth_thread_attr_t *attr,
				       size_t guardsize) {
  attr->guardsize = guardsize;
  return 0;
}

static inline int myth_thread_attr_getstacksize_body(const myth_thread_attr_t *attr,
				       size_t *stacksize) {
  *stacksize = attr->stacksize;
  return 0;
}

static inline int myth_thread_attr_setstacksize_body(myth_thread_attr_t *attr,
				       size_t stacksize) {
  attr->stacksize = stacksize;
  return 0;
}

static inline int myth_thread_attr_getstack_body(const myth_thread_attr_t *attr,
				   void **stackaddr, size_t *stacksize) {
  *stackaddr = attr->stackaddr;
  *stacksize = attr->stacksize;
  return 0;
}

static inline int myth_thread_attr_setstack_body(myth_thread_attr_t *attr,
				   void *stackaddr, size_t stacksize) {
  attr->stackaddr = stackaddr;
  attr->stacksize = stacksize;
  return 0;
}

static inline int myth_getattr_default_body(myth_thread_attr_t *attr) {
  return myth_thread_attr_init_body(attr);
}

static inline int myth_getattr_body(myth_thread_t thread, myth_thread_attr_t *attr) {
  *attr = thread->attr;
  return 0;
}

static inline int myth_getconcurrency_body(void) {
  return myth_get_num_workers_body();
}

/* --------
   yield
   -------- */

MYTH_CTX_CALLBACK void myth_yield_ex_1(void * arg1, void * arg2, void * arg3) {
  myth_running_env_t env = arg1;
  myth_thread_t this_thread = arg2;
  myth_thread_t next_thread = arg3;
  //Push current thread to the tail of runqueue
  myth_queue_put(&env->runnable_q, this_thread);
  env->this_thread = next_thread;
  next_thread->env = env;
}

//Yield execution to next runnable thread
static inline int myth_yield_ex_body(int opt) {
  int _ = myth_ensure_init();
  myth_running_env_t env = myth_get_current_env();
  myth_thread_t th = env->this_thread;
  myth_thread_t next;
  (void)_;
  myth_assert(th);
#if MYTH_YIELD_DEBUG
  myth_dprintf("myth_yield:thread %p yields execution to scheduler\n",th);
#endif
  //Get next runnable thread
  next = NULL;
  switch (opt) {
  case myth_yield_option_local_only: {
    next = myth_queue_pop(&env->runnable_q);
    break;
  }
  case myth_yield_option_local_first: {
    next = myth_queue_pop(&env->runnable_q);
    if (!next) {
      next = g_myth_steal_func(env->rank);
    }
    break;
  }
  case myth_yield_option_steal_only: {
    next = g_myth_steal_func(env->rank);
    break;
  }
  case myth_yield_option_steal_first: {
    next = g_myth_steal_func(env->rank);
    if (!next) {
      next = myth_queue_pop(&env->runnable_q);
    }
    break;
  }
  default:
    assert(0);
  }
  if (next) {
    next->env=env;
    //Switch context and push current thread to runqueue
    myth_swap_context_withcall(&th->context, &next->context,
			       myth_yield_ex_1,
			       (void*)env, (void*)th, (void*)next);
  }
#if MYTH_YIELD_DEBUG
  myth_dprintf("myth_yield:thread %p continues execution\n",th);
#endif
  return 0;
}

static inline int myth_yield_body(void) {
  return myth_yield_ex_body(myth_yield_option_local_first);
}

static inline void myth_timespec_add(const struct timespec * a,
				     const struct timespec * b,
				     struct timespec * c) {
  long ns = a->tv_nsec + b->tv_nsec;
  c->tv_nsec = ns % 1000000000;
  c->tv_sec = a->tv_sec + b->tv_sec + ns / 1000000000;
}

static inline int myth_timespec_gt(const struct timespec * a,
				   const struct timespec * b) {
  if (a->tv_sec > b->tv_sec) return 1;
  if (a->tv_sec == b->tv_sec) return a->tv_nsec > b->tv_nsec;
  return 0;
}

static inline int myth_nanosleep_body(const struct timespec *req,
				      struct timespec *rem) {
  struct timespec unt[1], cur[1];
  (void)rem;
  if (req->tv_sec < 0) return EINVAL;
  if (req->tv_nsec < 0) return EINVAL;
  if (req->tv_nsec > 999999999) return EINVAL;
  clock_gettime(CLOCK_REALTIME, cur);
  myth_timespec_add(cur, req, unt);
  while (1) {
    clock_gettime(CLOCK_REALTIME, cur);
    if (myth_timespec_gt(cur, unt)) break;
    myth_yield_body();
  }
  return 0;
}

static inline int myth_usleep_body(useconds_t usec) {
  struct timespec req[1];
  req->tv_sec = (usec / 1000000);
  req->tv_nsec = (usec % 1000000) * 1000;
  return myth_nanosleep_body(req, 0);
}

static inline unsigned int myth_sleep_body(unsigned int s) {
  struct timespec req[1];
  req->tv_sec = s;
  req->tv_nsec = 0;
  return myth_nanosleep_body(req, 0);
}

#if 0
MYTH_CTX_CALLBACK void myth_yield_1(void *arg1,void *arg2,void *arg3) {
  myth_running_env_t env=arg1;
  myth_thread_t this_thread=arg2,next_thread=arg3;
  {
    //Push current thread to the tail of runqueue
    myth_queue_put(&env->runnable_q,this_thread);
    env->this_thread=next_thread;
    next_thread->env=env;
  }
}

//Yield execution to next runnable thread
static inline void myth_yield_body(int force_worksteal)
{
  myth_running_env_t env;
  myth_thread_t th,next;
  int _ = myth_ensure_init();
  env = myth_get_current_env();
  th = env->this_thread;
  myth_assert(th);
#if MYTH_YIELD_DEBUG
  myth_dprintf("myth_yield:thread %p yields execution to scheduler\n",th);
#endif
  //Get next runnable thread
  next=NULL;
  if (force_worksteal){
    next=g_myth_steal_func(env->rank);
    if (!next)next=myth_queue_pop(&env->runnable_q);
  }
  else{
    next=myth_queue_pop(&env->runnable_q);
    if (!next)next=g_myth_steal_func(env->rank);
  }
  if (next){
    next->env=env;
    //Switch context and push current thread to runqueue
    myth_swap_context_withcall(&th->context,&next->context,myth_yield_1,(void*)env,(void*)th,(void*)next);
  }
#if MYTH_YIELD_DEBUG
  myth_dprintf("myth_yield:thread %p continues execution\n",th);
#endif
}

MYTH_CTX_CALLBACK void myth_yield2_1(void *arg1,void *arg2,void *arg3)
{
  myth_running_env_t env=arg1;
  myth_thread_t this_thread=arg2,next_thread=arg3;
  {
    //Push current thread to the tail of runqueue
    myth_queue_push(&env->runnable_q,this_thread);
    env->this_thread=next_thread;
    next_thread->env=env;
  }
}

//Yield execution to next runnable thread
static inline void myth_yield2_body(void)
{
  myth_running_env_t env;
  myth_thread_t th,next;
  env=myth_get_current_env();
  th=env->this_thread;
  myth_assert(th);
  //Get next runnable thread
  next=myth_queue_pop(&env->runnable_q);
  if (!next)next=g_myth_steal_func(env->rank);
  if (next){
    next->env=env;
    //Switch context and push current thread to runqueue
    myth_swap_context_withcall(&th->context,&next->context,myth_yield2_1,(void*)env,(void*)th,(void*)next);
  }
}
#endif	/* 0 */



//Entry point of threads
static void __attribute__((unused)) myth_entry_point(void)
{
  myth_thread_t this_thread;
  myth_running_env_t env;
#if MYTH_ENTRY_POINT_PROF
  uint64_t t0,t1;
  t0=myth_get_rdtsc();
#endif
  env=myth_get_current_env();
  this_thread=env->this_thread;
#if MYTH_ENTRY_POINT_PROF
  t1=myth_get_rdtsc();
  env->prof_data.ep_cyclesA+=t1-t0;
#endif
#if MYTH_ENTRY_POINT_DEBUG
  myth_dprintf("Running thread %p(arg:%p)\n",this_thread,this_thread->arg);
#endif
  //Execute a thread function
  this_thread->result=(*(this_thread->entry_func))(this_thread->result);
  myth_entry_point_cleanup(this_thread);
}

//Switch to next_thread
MYTH_CTX_CALLBACK void myth_entry_point_1(void *arg1,void *arg2,void *arg3)
{
  MAY_BE_UNUSED uint64_t t0, t1;
  myth_running_env_t env = arg1;
  myth_thread_t this_thread = arg2, next_thread = arg3;
  t0 = 0; t1 = 0;
#if MYTH_EP_PROF_DETAIL
  t1 = myth_get_rdtsc();
  env->prof_data.ep_switch += t1-env->prof_data.ep_d_tmp;
  t0 = myth_get_rdtsc();
#endif
  free_myth_thread_struct_stack(env,this_thread);
  if (this_thread->detached){
    //The thread is detached. Release resource
#if MYTH_ENTRY_POINT_DEBUG
    myth_dprintf("Thread %p is detached.Freed resource\n",this_thread);
#endif
    myth_spin_unlock_body(&this_thread->lock);
    free_myth_thread_struct_desc(env,this_thread);
  }
  else{
#if QUICK_CHECK_ON_JOIN
    this_thread->status = MYTH_STATUS_FREE_READY;
    myth_spin_unlock_body(&this_thread->lock);
    this_thread->status = MYTH_STATUS_FREE_READY2;
#else
    this_thread->status=MYTH_STATUS_FREE_READY2;
    myth_spin_unlock_body(&this_thread->lock);
#endif
  }
  env->this_thread = next_thread;
#if MYTH_EP_PROF_DETAIL
  t1=myth_get_rdtsc();
  env->prof_data.ep_join+=t1-t0;
  env->prof_data.ep_d_cnt++;
#endif
#if MYTH_ENTRY_POINT_PROF
  uint64_t t3;
  t3=myth_get_rdtsc();
  env->prof_data.ep_cnt++;
  env->prof_data.ep_cyclesB+=t3-env->prof_data.ep_cycles_tmp;
#endif
#if MYTH_SWITCH_PROF
  env->prof_data.sw_tmp=myth_get_rdtsc();
#endif
}

//Switch to the scheduler
MYTH_CTX_CALLBACK void myth_entry_point_2(void *arg1,void *arg2,void *arg3)
{
  MAY_BE_UNUSED uint64_t t0,t1;
  myth_running_env_t env=arg1;
  myth_thread_t this_thread=arg2;
  (void)arg3;
  t0=0;t1=0;
#if MYTH_EP_PROF_DETAIL
  t1=myth_get_rdtsc();
  env->prof_data.ep_switch+=t1-env->prof_data.ep_d_tmp;
  t0=myth_get_rdtsc();
#endif
  free_myth_thread_struct_stack(env,this_thread);
  if (this_thread->detached){
    //The thread is detached. Release resource
#if MYTH_ENTRY_POINT_DEBUG
    myth_dprintf("Thread %p is detached.Freed resource\n",this_thread);
#endif
    myth_spin_unlock_body(&this_thread->lock);
    free_myth_thread_struct_desc(env,this_thread);
  }
  else{
#if QUICK_CHECK_ON_JOIN
    this_thread->status=MYTH_STATUS_FREE_READY;
    myth_spin_unlock_body(&this_thread->lock);
    this_thread->status=MYTH_STATUS_FREE_READY2;
#else
    this_thread->status=MYTH_STATUS_FREE_READY2;
    myth_spin_unlock_body(&this_thread->lock);
#endif
  }
#if MYTH_EP_PROF_DETAIL
  t1=myth_get_rdtsc();
  env->prof_data.ep_join+=t1-t0;
  env->prof_data.ep_d_cnt++;
#endif
#if MYTH_ENTRY_POINT_PROF
  uint64_t t3;
  t3=myth_get_rdtsc();
  env->prof_data.ep_cnt++;
  env->prof_data.ep_cyclesB+=t3-env->prof_data.ep_cycles_tmp;
#endif
}

static inline void myth_entry_point_cleanup(myth_thread_t this_thread) {
  myth_tls_tree_fini(this_thread->tls, g_myth_tls_key_allocator);
  myth_running_env_t env;
  myth_thread_t this_thread_v;
#if MYTH_EP_PROF_DETAIL
  MAY_BE_UNUSED uint64_t t0,t1;
  t0 = 0; t1 = 0;
#endif
#if MYTH_ENTRY_POINT_DEBUG
  myth_dprintf("Finished thread %p\n",this_thread);
#endif
#if MYTH_ENTRY_POINT_PROF
  uint64_t t2 = myth_get_rdtsc();
#endif
#if MYTH_EP_PROF_DETAIL
  t0 = myth_get_rdtsc();
#endif
  //Get worker thread descriptor
  env = this_thread->env;
  myth_assert(this_thread->env == env);
  myth_assert(this_thread == env->this_thread);
#if MYTH_ENTRY_POINT_PROF
  env->prof_data.ep_cycles_tmp = t2;
#endif
  this_thread_v = this_thread;
  myth_spin_lock_body(&this_thread->lock);
  myth_thread_t wait_thread = this_thread_v->join_thread;
  //Execute a thread waiting for current thread
  if (wait_thread){
#if MYTH_DEBUG_JOIN_FCC
    this_thread->finished_at = myth_get_rdtsc();
    this_thread->when_I_finished = "there was a waiter";
    this_thread->waiter = wait_thread;
#endif
    //sanity check
    myth_assert(wait_thread->status == MYTH_STATUS_BLOCKED);
    wait_thread->env = env;
    wait_thread->status = MYTH_STATUS_READY;
#if MYTH_ENTRY_POINT_DEBUG
    myth_dprintf("Join process completed %p\n",this_thread);
#endif
#if SWITCH_AFTER_EXIT
#if MYTH_EP_PROF_DETAIL
    t1 = myth_get_rdtsc();
    env->prof_data.ep_join += t1 - t0;
    env->prof_data.ep_d_tmp = myth_get_rdtsc();
#endif
    //Execute
    myth_set_context_withcall(&wait_thread->context,
			      myth_entry_point_1,
			      (void*)env, this_thread, wait_thread);
#else
    //Push to the runqueue
    myth_queue_push(&env->runnable_q, wait_thread);
#endif
  }
#if MYTH_EP_PROF_DETAIL
  t1 = myth_get_rdtsc();
  env->prof_data.ep_join += t1 - t0;
  t0 = myth_get_rdtsc();
#endif
  
  //Get next runnable thread
  myth_thread_t next = myth_queue_pop(&env->runnable_q);

#if MYTH_EP_PROF_DETAIL
  t1 = myth_get_rdtsc();
  env->prof_data.ep_pop += t1 - t0;
#endif
  if (next){
#if MYTH_EP_PROF_DETAIL
    env->prof_data.ep_d_tmp=myth_get_rdtsc();
#endif
#if MYTH_DEBUG_JOIN_FCC
    this_thread->finished_at = myth_get_rdtsc();
    this_thread->when_I_finished = "no waiter and go to next";
    this_thread->waiter = 0;
#endif
    //Switch to the next thread
    myth_set_context_withcall(&next->context, myth_entry_point_1,
			      (void*)env, this_thread, next);
  } else {
#if MYTH_EP_PROF_DETAIL
    env->prof_data.ep_d_tmp = myth_get_rdtsc();
#endif
#if MYTH_DEBUG_JOIN_FCC
    this_thread->finished_at = myth_get_rdtsc();
    this_thread->when_I_finished = "no waiter and go to sched";
    this_thread->waiter = 0;
#endif
    //Switch to the scheduler
    myth_set_context_withcall(&env->sched.context, myth_entry_point_2,
			      (void*)env, this_thread, NULL);
  }
  if (next) {
    myth_entry_point_1((void*)env, this_thread, next);
  } else {
    myth_entry_point_2((void*)env, this_thread, NULL);
  }
}

static inline int myth_cancel_body(myth_thread_t th) {
  //send cancel request
  myth_spin_lock_body(&th->lock);
  th->cancelled = 1;
  myth_spin_unlock_body(&th->lock);
  return 0;
}

static inline int myth_setcancelstate_body(int state, int *oldstate) {
  //enable/disable cancel
  myth_thread_t th = myth_self_body();
  myth_spin_lock_body(&th->lock);
  if (oldstate) {
    *oldstate = (th->cancel_enabled) ? MYTH_CANCEL_ENABLE : MYTH_CANCEL_DISABLE;
  }
  th->cancel_enabled = (state == MYTH_CANCEL_ENABLE) ? 1 : 0;
  myth_spin_unlock_body(&th->lock);
  return 0;
}

static inline int myth_setcanceltype_body(int type,int *oldtype) {
  //Allow deferred cancel only
  if (type != MYTH_CANCEL_DEFERRED)
    return EINVAL;
  if (oldtype)
    *oldtype = MYTH_CANCEL_DEFERRED;
  return 0;
}

static inline int myth_is_canceled(myth_thread_t th) {
  int c;
  //Is a thread cancelled?
  myth_spin_lock_body(&th->lock);
  c = (th->cancel_enabled && th->cancelled) ? 1 : 0;
  myth_spin_unlock_body(&th->lock);
  return c;
}

static inline void myth_testcancel_body(void) {
  myth_thread_t th = myth_self_body();
  int c = myth_is_canceled(th);
  if (c) {
    //set return value as cancelled
    th->result = MYTH_CANCELED;
    //exit
    myth_entry_point_cleanup(th);
  }
}

static inline void myth_sched_prof_start_body(void) {
  g_sched_prof=1;
}

static inline void myth_sched_prof_pause_body(void) {
  g_sched_prof=0;
}

#if 0

//Release the stack of a stolen thread
static inline void myth_release_stack_body(myth_thread_t th)
{
  myth_running_env_t e=myth_get_current_env();
  free_myth_thread_struct_stack(e,th);
}

//Release the thread descriptor of a stolen thread
static inline void myth_release_desc_body(myth_thread_t th)
{
  myth_running_env_t e=myth_get_current_env();
  free_myth_thread_struct_desc(e,th);
}

//Serialize
static inline void myth_serialize_body(myth_thread_t th,myth_pickle_t p)
{
  //Copy descriptor
  memcpy(&p->desc,th,sizeof(myth_thread));
  //スタックをコピー
  memcpy(&p->stack,(void*)(((char*)th->stack)-(PAGE_ALIGNED_STACK_SIZE-sizeof(void*))),PAGE_ALIGNED_STACK_SIZE);
}

//Try de-serialize
static inline myth_thread_t myth_deserialize_body(myth_pickle_t p)
{
  void *stack_ptr;
  myth_running_env_t e;
  e=myth_get_current_env();
  myth_thread_t ret;
  //Try to mmap with fixed address
  void *stack_start_addr;
  stack_start_addr=(void*)( ((char*)p->desc.stack)-(PAGE_ALIGNED_STACK_SIZE-sizeof(void*)) );
#if defined(MAP_STACK)
  stack_ptr=myth_mmap(stack_start_addr,PAGE_ALIGNED_STACK_SIZE,PROT_READ|PROT_WRITE,MAP_FIXED|MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK,-1,0);
#else
  stack_ptr=myth_mmap(stack_start_addr,PAGE_ALIGNED_STACK_SIZE,PROT_READ|PROT_WRITE,MAP_FIXED|MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
#endif
  //Error if allocation fails
  if (stack_ptr==MAP_FAILED)return NULL;
  myth_assert(stack_ptr!=p->desc.stack);
  //Allocate a new descriptor
  ret=get_new_myth_thread_struct_desc(e);
  //Copy the desctiptor and the stack
  memcpy(ret,&p->desc,sizeof(myth_thread));
  memcpy(stack_ptr,p->stack,PAGE_ALIGNED_STACK_SIZE);
  return ret;
}

static inline myth_thread_t myth_ext_deserialize_body(myth_pickle_t p)
{
  void *stack_ptr;
  MAY_BE_UNUSED myth_running_env_t e;
  e=myth_get_current_env();
  myth_thread_t ret;
  //Try to mmap with fixed address
  void *stack_start_addr;
  stack_start_addr=(void*)( ((char*)p->desc.stack)-(PAGE_ALIGNED_STACK_SIZE-sizeof(void*)) );
#if defined(MAP_STACK)
  stack_ptr=myth_mmap(stack_start_addr,PAGE_ALIGNED_STACK_SIZE,PROT_READ|PROT_WRITE,MAP_FIXED|MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK,-1,0);
#else
  stack_ptr=myth_mmap(stack_start_addr,PAGE_ALIGNED_STACK_SIZE,PROT_READ|PROT_WRITE,MAP_FIXED|MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
#endif
  //Error if allocation fails
  if (stack_ptr==MAP_FAILED)return NULL;
  myth_assert(stack_ptr!=p->desc.stack);
  //Allocate a new descriptor
  ret=get_new_myth_thread_struct_desc_ext();
  //Copy the desctiptor and the stack
  memcpy(ret,&p->desc,sizeof(myth_thread));
  memcpy(stack_ptr,p->stack,PAGE_ALIGNED_STACK_SIZE);
  return ret;
}

#endif

#include "myth_io_func.h"

#endif /* MYTH_FUNC_H_ */
