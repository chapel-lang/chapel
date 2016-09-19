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
#include <new>

/* 
 * 
 */

#if !defined(TO_SERIAL) && !defined(TO_MTHREAD) && !defined(TO_MTHREAD_NATIVE) && !defined(TO_TBB) && !defined(TO_QTHREAD) && !defined(TO_NANOX)
/* from Nov 8. 2013, native is default */
#define TO_MTHREAD_NATIVE 1
/* the default used to be pthread-compatible MassiveThreads */
// #define TO_MTHREAD 1
#endif


/* include files */
#if TO_SERIAL

#elif TO_MTHREAD
#include <pthread.h>
#elif TO_MTHREAD_NATIVE
#include <myth/myth.h>
#elif TO_QTHREAD 
#include <qthread.h>
#elif TO_NANOX
#include <nanos.h>
#elif TO_TBB
/* odd enough? simply pass through task_group to tbb's one */
#include <tbb/task_group.h>
#else
#error "none of TO_SERIAL/TO_QTHREAD/TO_NANOX/TO_MTHREAD/TO_MTHREAD_NATIVE defined"
#endif

/* the return type of a thread-invoking function
   (e.g., pthread_create returns void *) */
#if TO_SERIAL
#define th_func_ret_type void *
#elif TO_MTHREAD
#define th_func_ret_type void *
#elif TO_MTHREAD_NATIVE
#define th_func_ret_type void *
#elif TO_QTHREAD
#define th_func_ret_type aligned_t
#elif TO_NANOX
#define th_func_ret_type void
#endif

#if !defined(TASK_GROUP_INIT_SZ)
#define TASK_GROUP_INIT_SZ 8
#endif

/* if set to one, it "serializes" task
   creations, no matter which underlying 
   system you use */
#if !defined(TASK_GROUP_NULL_CREATE)
#define TASK_GROUP_NULL_CREATE 0
#endif


#if !defined(TASK_MEMORY_CHUNK_SZ)
#define TASK_MEMORY_CHUNK_SZ 256
#endif

#if !defined(DAG_RECORDER)
#define DAG_RECORDER_VER 0	/* old version */
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include <dag_recorder.h>
#ifdef __cplusplus
}
#endif

namespace mtbb {

#if TO_TBB

  /* I don't remember why this is necessary... */
  typedef tbb::task task;

  /* even with TBB, we still wrap the original
     task_group class, with one that defines 
     run_ and wait_ methods, which take extra 
     file and line parameters.  
     they are used only when DAG_RECORDER=2.
     in order to simplify matters, they are
     defined even when DAG_RECORDER!=2.
  */
  // typedef tbb::task_group task_group_no_prof;

  struct task_group_no_prof : tbb::task_group {
    template<typename C>
      void run_(C c, const char * file, int line) {
      (void)file;
      (void)line;
      run(c);
    }

    void wait_(const char * file, int line) {
      (void)file;
      (void)line;
      wait();
    }
  };

  
#else  /* anything but TBB */


  /* repreesntation of a task. 
     a subclass of a task should implement 
     execute() method, just as in tbb */
  struct task {
    virtual void * execute() = 0;
#if TO_SERIAL
    
#elif TO_MTHREAD
    pthread_t tid;
#elif TO_MTHREAD_NATIVE
    myth_thread_t hthread;
#elif TO_QTHREAD
    aligned_t ret;
#elif TO_NANOX
    
#else
#error "none of TO_SERIAL/TO_QTHREAD/TO_NANOX/TO_MTHREAD/TO_MTHREAD_NATIVE defined"
#endif
  };

  /* a wrapper that makes any callable object a task */
  template<typename F>
  struct callable_task : task {
    F f;
    callable_task(F f_) : f(f_) {}
    void * execute() { f(); return NULL; }
  };

  /* an implementation of a list of tasks;
     used inside task_group class to store 
     outstanding tasks */
  struct task_list_node {
    task_list_node * next;
    int capacity;
    int n;
    task * a[TASK_GROUP_INIT_SZ];
    void init() {
      next = NULL;
      capacity = TASK_GROUP_INIT_SZ;
      n = 0;
    }
  };
  
  struct task_list {
    task_list_node head[1];
    task_list_node * tail;

    void init() {
      head->init();
      tail = head;
    }
    void reset() {
      task_list_node * q = NULL;
      for (task_list_node * p = head->next; p; p = q) {
	q = p->next;
	delete p;
      }
      init();
    }
    void new_node() {
      task_list_node * new_node_ = new task_list_node();
      new_node_->init();
      tail->next = new_node_;
      tail = new_node_;
    }
    task * add(task * t) {
      if (tail->n == tail->capacity) new_node();
      tail->a[tail->n] = t;
      tail->n++;
      return t;
    }
  };

  /* roll a memory allocator for tasks, so that
     we call system malloc (new) only occasionally */
  struct task_memory_chunk {
    char a_[TASK_MEMORY_CHUNK_SZ];
    char * a;
    char * p;			/* allocation ptr */
    char * end;
    task_memory_chunk * next;
    void init(size_t s) {
      if (s <= TASK_MEMORY_CHUNK_SZ) {
	s = TASK_MEMORY_CHUNK_SZ;
	a = a_;
      } else {
	a = new char[s];
      }
      end = a + s;
      p = a;
      next = 0;
    }
    ~task_memory_chunk() {
      if (a != a_) delete a;
    }
  };

  struct task_memory_allocator {
    task_memory_chunk head[1];
    task_memory_chunk * tail;
    void init() { 
      head->init(TASK_MEMORY_CHUNK_SZ);
      tail = head;
    }
    char * new_chunk(size_t s) {
      task_memory_chunk * ch = new task_memory_chunk();
      ch->init(s);
      tail->next = ch;
      tail = ch;
      return ch->p;
    }
    void * alloc(size_t s) {
      char * p = tail->p;
      if (p + s > tail->end)
	p = new_chunk(s);
      assert(tail->p == p);
      assert(tail->p + s <= tail->end);
      tail->p = p + s;
      return (void *)p;
    }
    void reset() {
      task_memory_chunk * q = NULL;
      for (task_memory_chunk * p = head->next; p; p = q) {
	q = p->next;
	delete p;
      }
      init();
    }
  };

  /* receive a task object and invoke it.
     invoke_task is the one passed to the 
     underlying thread creation function.
     e.g., pthread_create(invoke_task, task_obj) */
  static th_func_ret_type invoke_task(void * arg_) {
    task * arg = (task *)arg_;
    arg->execute();
#if TO_SERIAL || TO_MTHREAD || TO_QTHREAD || TO_MTHREAD_NATIVE
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

  /* task group object. no profiler version */
  struct task_group_no_prof {
    task_list tasks;
    task_memory_allocator mem;
    task_group_no_prof() {
      tasks.init();
      mem.init();
    }

    void 
    run_task(task * t)  
    {
#if EASY_PAPI2 || EASY_PAPI
      /* if you get a "no such function" error,
	 it's because you didn't include easy_papi2.c 
	 BEFORE you include this file. */
      epapi_read();
#endif
      tasks.add(t);
#if TASK_GROUP_NULL_CREATE
      invoke_task((void *)t);
#elif TO_SERIAL
      invoke_task((void *)t);
#elif TO_MTHREAD
      pthread_create(&t->tid, NULL, invoke_task, (void*)t);
#elif TO_MTHREAD_NATIVE
      t->hthread=myth_create(invoke_task,(void*)t);
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
#else
#error "none of TO_QTHREAD/TO_NANOX/TO_MTHREAD/TO_MTHREAD_NATIVE defined"
#endif
    }

    void run_task_if(bool x, task * t) {
      if (x) run_task(t);
      else t->execute();
    }

    /* run any callable object (e.g., closure) */
    template<typename C>
    void run(C c) {
      void * a = mem.alloc(sizeof(mtbb::callable_task<C>));
      mtbb::callable_task<C> * ct = new (a) mtbb::callable_task<C>(c);
      run_task(ct);
    }

    /* run if x is true. otherwise call it in serial */
    template<typename C>
    void run_if(bool x, C c) {
      if (x) c();
      else run(c);
    }

    void wait() {
      int n_joined = 0;
#if EASY_PAPI2 || EASY_PAPI
      epapi_read();
#endif
      for (task_list_node * p = tasks.head; p; p = p->next) {
	for (int i = 0; i < p->n; i++) {
#if TASK_GROUP_NULL_CREATE 
	  /* noop */
#elif TO_SERIAL
	  /* noop */
#elif TO_MTHREAD
	  pthread_join(p->a[i]->tid, NULL);
#elif TO_MTHREAD_NATIVE
	  myth_join(p->a[i]->hthread,NULL);
#elif TO_QTHREAD
	  aligned_t ret;
	  qthread_readFF(&ret,&p->a[i]->ret);
#elif TO_NANOX
	  if (n_joined == 0) {
	    NANOS_SAFE(nanos_wg_wait_completion(nanos_current_wd(), 1));
	  }
#else

#error "none of TO_QTHREAD/TO_NANOX/TO_MTHREAD/TO_MTHREAD_NATIVE defined"

#endif
	  n_joined++;
	}
      }
      tasks.reset();
      mem.reset();
    }

    template<typename C>
    void run_(C c, const char * file, int line) {
      (void)file;
      (void)line;
      run(c);
    }

    void wait_(const char * file, int line) {
      (void)file;
      (void)line;
      wait();
    }
  };

#endif	/* TO_TBB. have defined task_group_no_prof */


#if DAG_RECORDER>=2
/* task_group with dag profiler*/
#if defined(dr_get_worker)
/* you have your own dr_get_worker */
#elif TASK_GROUP_NULL_CREATE 
#define dr_get_worker() 0
#define dr_get_max_workers() 1
#elif TO_SERIAL
#define dr_get_worker() 0
#define dr_get_max_workers() 1
#elif TO_MTHREAD
#define dr_get_worker() myth_get_worker_num()
#define dr_get_max_workers() myth_get_num_workers()
#elif TO_MTHREAD_NATIVE
#define dr_get_worker() myth_get_worker_num()
#define dr_get_max_workers() myth_get_num_workers()
#elif TO_QTHREAD
#define dr_get_worker() qthread_worker(NULL)
#define dr_get_max_workers() qthread_num_workers()
#elif TO_TBB
#define dr_get_worker() dr_tbb_get_worker()
#define dr_get_max_workers() dr_tbb_max_workers()
#elif TO_NANOX
#define dr_get_worker() dr_nanox_get_worker()
#define dr_get_max_workers() dr_nanox_max_workers()
#else
#error "DAG_RECORDER==2 requires a definition of dr_get_worker(). define one, or use predefined one by defining one of TO_SERIAL, TO_MTHREAD, TO_MTHREAD_NATIVE, TO_QTHREAD, TO_TBB, TO_NANOX"
#endif

  template<typename Callable>
    struct dr_wrap_callable {
      Callable c;
      dr_dag_node * parent_interval;
      const char * file;
      long line;
    dr_wrap_callable(Callable c_, dr_dag_node * parent_interval_,
		     const char * file_, long line_) :
      c(c_), parent_interval(parent_interval_),
	file(file_), line(line_) {}
      
      void operator() () {
	dr_start_task_(parent_interval, file, line);
	c();
	dr_end_task_(file, line);
      }
    };

  struct task_group_with_prof : public task_group_no_prof {
    int n_outstanding_children;
    task_group_with_prof() {
      task_group_no_prof();
      n_outstanding_children = 0;
    }

    template <typename Callable>
      void run_(Callable c, const char * file, int line) {
#if EASY_PAPI2 || EASY_PAPI
      epapi_read();
#endif
      if (n_outstanding_children == 0) dr_begin_section();
      n_outstanding_children++;
      dr_dag_node * ci = 0;
      dr_dag_node * t = dr_enter_create_task_(&ci, file, line);
      task_group_no_prof::run(dr_wrap_callable<Callable>(c, ci, file, line));
      dr_return_from_create_task_(t, file, line);
    }

    template <typename Callable>
      void run(Callable c) {
      run_(c, __FILE__, __LINE__);
    }

    void wait_(const char * file, int line) {
#if EASY_PAPI2 || EASY_PAPI
      epapi_read();
#endif
      if (n_outstanding_children == 0) dr_begin_section();
      dr_dag_node * t = dr_enter_wait_tasks_(file, line);
      task_group_no_prof::wait();
      n_outstanding_children = 0;
      dr_return_from_wait_tasks_(t, file, line);
    }

    void wait() {
      wait_(__FILE__, __LINE__);
    }
  };
  typedef task_group_with_prof task_group;

#else

  typedef task_group_no_prof task_group;

#endif

} /* namespace mtbb */

