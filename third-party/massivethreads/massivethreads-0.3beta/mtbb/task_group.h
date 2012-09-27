#pragma once
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <functional>

#if QTHREAD 
#include <qthread.h>
#elif NANOX
#include <nanos.h>
#else
#include <pthread.h>
#define PTHREAD_LIKE 1
#endif

#if PTHREAD_LIKE
#define th_func_ret_type void *
#elif QTHREAD
#define th_func_ret_type aligned_t
#elif NANOX
#define th_func_ret_type void
#endif

#if !defined(TASK_GROUP_INIT_SZ)
#define TASK_GROUP_INIT_SZ 10
#endif

#if !defined(TASK_GROUP_NULL_CREATE)
#define TASK_GROUP_NULL_CREATE 0
#endif

struct task {
  pthread_t tid;
  std::function<void ()> f;
#if QTHREAD
  aligned_t ret;
#endif
};

struct task_list_node {
  task_list_node * next;
  int capacity;
  int n;
  task a[TASK_GROUP_INIT_SZ];
};

th_func_ret_type invoke_task(void * arg_) {
  task * arg = (task *)arg_;
  std::function<void()> f = arg->f;
  f();
#if PTHREAD_LIKE || QTHREAD
  return 0;
#endif
}

#if NANOX
nanos_smp_args_t invoke_task_arg={invoke_task};
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
#if PTHREAD_LIKE
      pthread_create(&t->tid, NULL, invoke_task, (void*)t);
#elif QTHREAD
      qthread_fork(invoke_task, (void*)t, &t->ret);
#elif NANOX
      nanos_wd_t wd=NULL;
      nanos_device_t dev[1] = {NANOS_SMP_DESC(invoke_task_arg)};
      nanos_wd_props_t props;	// originally, ={true,false,false};
      props.mandatory_creation = true;
      props.tied = false;
      props.reserved0 = false;
      NANOS_SAFE(nanos_create_wd(&wd,1,dev,sizeof(struct task),
				 __alignof__(struct task),
				 (void**)&t,nanos_current_wd(),&props,0,NULL));
      NANOS_SAFE(nanos_submit(wd,0,0,0));
#else
#error "neither PTHREAD_LIKE, QTHREAD, nor NANOX defined"
#endif
    }
  }

  void wait() {
    int n_joined = 0;
    for (task_list_node * p = head; p && p->n; p = p->next) {
      for (int i = 0; i < p->n; i++) {
#if TASK_GROUP_NULL_CREATE 
	/* noop */
#elif PTHREAD_LIKE
	void * ret;
	pthread_join(p->a[i].tid, &ret);
#elif QTHREAD
	aligned_t ret;
	qthread_readFF(&ret,&p->a[i].ret);
#elif NANOX
	if (n_joined == 0) {
	  NANOS_SAFE(nanos_wg_wait_completion(nanos_current_wd()));
	}
#else
#error "neither PTHREAD_LIKE, QTHREAD, nor NANOX defined"
#endif
	n_joined++;
      }
      p->n = 0;
    }
  }
};
