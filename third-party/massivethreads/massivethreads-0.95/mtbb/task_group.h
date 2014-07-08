/* 
 * task_group.h
 */

/* 
 * this file provides a class task_group compatible to
 * the task_group class in the Intel Threading Building Block.
 * see the following TBB "design pattern" page to learn what
 * task_group class provides.
 http://software.intel.com/sites/products/documentation/doclib/tbb_sa/help/tbb_userguide/Design_Patterns/Divide_and_Conquer.htm

 * unlike TBB, scheduler is a genuine, greedy work-first work 
 * stealing scheduler, which tends to have a better scalability
 * than TBB.
 * 
 */

#pragma once
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <functional>

/* 
 * 
 */

#if !defined(TO_MTHREAD) && !defined(TO_MTHREAD_NATIVE) && !defined(TO_QTHREAD) && !defined(TO_NANOX)
#define TO_MTHREAD 1
#endif

#if TO_QTHREAD 
#include <qthread.h>
#elif TO_NANOX
#include <nanos.h>
#elif TO_MTHREAD
#include <pthread.h>
#elif TO_MTHREAD_NATIVE
#include <myth.h>
#else

#error "none of TO_QTHREAD/TO_NANOX/TO_MTHREAD/TO_MTHREAD_NATIVE defined"

#endif

#if TO_MTHREAD
#define th_func_ret_type void *
#elif TO_QTHREAD
#define th_func_ret_type aligned_t
#elif TO_NANOX
#define th_func_ret_type void
#elif TO_MTHREAD_NATIVE
#define th_func_ret_type void *
#endif

#if !defined(TASK_GROUP_INIT_SZ)
#define TASK_GROUP_INIT_SZ 10
#endif

#if !defined(TASK_GROUP_NULL_CREATE)
#define TASK_GROUP_NULL_CREATE 0
#endif


namespace mtbb {

  struct task {
    std::function<void ()> f;
#if TO_MTHREAD
    pthread_t tid;
#elif TO_QTHREAD
    aligned_t ret;
#elif TO_MTHREAD_NATIVE
    myth_thread_t hthread;
#elif TO_NANOX
    
#else
    
#error "none of TO_QTHREAD/TO_NANOX/TO_MTHREAD/TO_MTHREAD_NATIVE defined"
    
#endif
  };

  struct task_list_node {
    task_list_node * next;
    int capacity;
    int n;
    task a[TASK_GROUP_INIT_SZ];
  };
  
  static th_func_ret_type invoke_task(void * arg_) {
    task * arg = (task *)arg_;
    std::function<void()> f = arg->f;
    f();
#if TO_MTHREAD || TO_QTHREAD || TO_MTHREAD_NATIVE
    return 0;
#elif TO_NANOX
    
#else
    
#error "none of TO_QTHREAD/TO_NANOX/TO_MTHREAD/TO_MTHREAD_NATIVE defined"
    
#endif
  }
  
#if TO_NANOX
  static nanos_smp_args_t invoke_task_arg={invoke_task};
  
  struct nanos_const_wd_definition_for_task {
    nanos_const_wd_definition_t base;
    nanos_device_t devices[1];
  };
  
  static struct nanos_const_wd_definition_for_task wd_definition_for_task = {
    {
      {
	0,			/* mandatory_creation */
	0,			/* tied */
	0,0,0,0,0,0		/* reserved0-5 */
      },
      __alignof__(struct task),	/* data_alignment */
      0,			/* num_copies */
      1,			/* num_devices */
      1,			/* num_dimensions */
      NULL			/* description */
    },
    {
      {
	nanos_smp_factory,
	&invoke_task_arg
      }
    }
  };
#endif
  

  struct task_group {
    task_list_node first_chunk_[1];
    task_list_node * head;
    task_list_node * tail;
    task_group() {
      head = first_chunk_;
      tail = first_chunk_;
      head->next = NULL;
      head->capacity = TASK_GROUP_INIT_SZ;
      head->n = 0;
    }
    ~task_group() {
      task_list_node * q = NULL;
      for (task_list_node * p = head; p; p = q) {
	q = p->next;
	if (p != first_chunk_) delete p;
      }
    }
    
    void extend() {
      task_list_node * new_node = new task_list_node();
      new_node->next = NULL;
      new_node->n = 0;
      new_node->capacity = TASK_GROUP_INIT_SZ;
      tail->next = new_node;
      tail = new_node;
    }
    void run(std::function<void ()> f) {
      if (tail->n == tail->capacity) {
	if (tail->next == NULL) extend();
	else tail = tail->next;
	assert(tail->n == 0);
      }
      task * t = &tail->a[tail->n];
      t->f = f;
      if (TASK_GROUP_NULL_CREATE) {
	invoke_task((void *)t);
      } else {
	tail->n++;
#if TO_MTHREAD
	pthread_create(&t->tid, NULL, invoke_task, (void*)t);
#elif TO_QTHREAD
	qthread_fork(invoke_task, (void*)t, &t->ret);
#elif TO_NANOX
	nanos_wd_t wd=NULL;
	nanos_wd_dyn_props_t dyn_props = { { 0,0,0,0,0,0,0,0 }, 0, 0 };
	NANOS_SAFE(nanos_create_wd_compact(&wd,&wd_definition_for_task.base,
					   &dyn_props,
					   sizeof(struct task), (void**)&t,
					   nanos_current_wd(),NULL,NULL));
	
	
#if OBSOLETE_NANOS
	/* Nanos at some prior versions. obsolete  */
	nanos_device_t dev[1] = {NANOS_SMP_DESC(invoke_task_arg)};
	nanos_wd_props_t props;	// originally, ={true,false,false};
	props.mandatory_creation = true;
	props.tied = false;
	props.reserved0 = false;
	NANOS_SAFE(nanos_create_wd(&wd,1,dev,sizeof(struct task),
				   __alignof__(struct task),
				   (void**)&t,nanos_current_wd(),&props,0,NULL));
#endif
	
	
	NANOS_SAFE(nanos_submit(wd,0,0,0));
#elif TO_MTHREAD_NATIVE
	t->hthread=myth_create(invoke_task,(void*)t);
#else
	
#error "none of TO_QTHREAD/TO_NANOX/TO_MTHREAD/TO_MTHREAD_NATIVE defined"
	
#endif
      }
    }

    void wait() {
      int n_joined = 0;
      for (task_list_node * p = head; p && p->n; p = p->next) {
	for (int i = 0; i < p->n; i++) {
#if TASK_GROUP_NULL_CREATE 
	  /* noop */
#elif TO_MTHREAD
	  pthread_join(p->a[i].tid, NULL);
#elif TO_QTHREAD
	  aligned_t ret;
	  qthread_readFF(&ret,&p->a[i].ret);
#elif TO_NANOX
	  if (n_joined == 0) {
	    NANOS_SAFE(nanos_wg_wait_completion(nanos_current_wd(), 1));
	  }
#elif TO_MTHREAD_NATIVE
	  myth_join(p->a[i].hthread,NULL);
#else

#error "none of TO_QTHREAD/TO_NANOX/TO_MTHREAD/TO_MTHREAD_NATIVE defined"

#endif
	  n_joined++;
	}
	p->n = 0;
      }
    }
  };

} /* namespace mtbb */

