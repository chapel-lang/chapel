/* 
 * cilk or cilkplus + dag recorder
 */

#pragma once

#include <cilk/cilk.h>

#if DAG_RECORDER>=2
#include <dag_recorder.h>
#endif

#define CILK_UNIFY_KEYWORDS 1

#if CILK_UNIFY_KEYWORDS
#define spawn cilk_spawn
#define sync cilk_sync
#endif

/* cilk interface; each cilk procedure (a procedure
   that calls spawn or sync) should look like:

cilk int f(int x) {
  cilk_begin;
  spawn_(cilk_spawn f(x));
  spawn_(y = cilk_spawn g(x));
  cilk_sync_;
  cilk_return(y);
}

  - you put cilk begin when it begins
   (syntactically, it is a variable declaration,
   so you can put other variable decls after it)

  - spawn f(x); -> spawn_(cilk_spawn f(x));
  - sync        -> cilk_sync_;

  - return x;   -> cilk_return(x)
  - return;     -> cilk_void_return;

 */

#define cilk_begin_no_prof			\
  int __cilk_begin__ = 0

#define cilk_return_no_prof(x) do {			   \
    (void)__cilk_begin__;				   \
    return x;						   \
  } while(0)

#define cilk_void_return_no_prof do {			   \
    (void)__cilk_begin__;				   \
    return;						   \
  } while(0)


#define spawn_no_prof(spawn_stmt) spawn_stmt

#define sync_no_prof do {				       \
    (void)__cilk_begin__;				       \
    cilk_sync;						       \
  } while(0)


#define cilk_begin_with_prof			\
  int __cilk_begin__ = dr_start_cilk_proc()

#define cilk_return_with_prof_t(type_of_x, x) do {	   \
    type_of_x __cilk_return_value__ = x;		   \
    (void)__cilk_begin__;				   \
    dr_end_task();					   \
    return __cilk_return_value__;			   \
  } while(0)


#define cilk_return_with_prof(x) do {			   \
    typeof(x) __cilk_return_value__ = x;		   \
    (void)__cilk_begin__;				   \
    dr_end_task();					   \
    return __cilk_return_value__;			   \
  } while(0)

#define cilk_void_return_with_prof do {		   \
    (void)__cilk_begin__;				   \
    dr_end_task();					   \
    return;						   \
  } while(0)

#define spawn_with_prof(spawn_stmt) do {			\
    dr_dag_node * __t__ = dr_enter_create_cilk_proc_task();	\
    (void)__cilk_begin__;					\
    spawn_stmt;							\
    dr_return_from_create_task(__t__);				\
  } while (0)

#define spawn_with_prof_(spawn_stmt, file, line) do {                   \
    dr_dag_node * __t__ = dr_enter_create_cilk_proc_task_(file, line);  \
    (void)__cilk_begin__;                                               \
    spawn_stmt;                                                         \
    dr_return_from_create_task_(__t__, file, line);                     \
  } while (0)

#define sync_with_prof do {				       \
    dr_dag_node * __t__ = dr_enter_wait_tasks();	       \
    (void)__cilk_begin__;				       \
    cilk_sync;						       \
    dr_return_from_wait_tasks(__t__);			       \
  } while(0)

#define sync_with_prof_(file, line) do {                       \
    dr_dag_node * __t__ = dr_enter_wait_tasks_(file, line);    \
    (void)__cilk_begin__;				       \
    cilk_sync;						       \
    dr_return_from_wait_tasks_(__t__, file, line);             \
  } while(0)


#if DAG_RECORDER>=2

#define cilk_begin            cilk_begin_with_prof
#define cilk_return(x)        cilk_return_with_prof(x)
#define cilk_return_t(type_of_x, x)        cilk_return_with_prof_t(type_of_x, x)
#define cilk_void_return      cilk_void_return_with_prof
#define spawn_(spawn_stmt)    spawn_with_prof(spawn_stmt)
#define spawn__(spawn_stmt, file, line)    spawn_with_prof_(spawn_stmt, file, line)
#define cilk_sync_            sync_with_prof
#define _Cilk_sync_           sync_with_prof
#define sync_                 sync_with_prof
#define sync__(file, line)    sync_with_prof_(file, line)

#define dr_get_max_workers()     __cilkrts_get_nworkers()
#define dr_get_worker()          __cilkrts_get_worker_number()

#else

#define cilk_begin            cilk_begin_no_prof
#define cilk_return(x)        cilk_return_no_prof(x)
#define cilk_return_t(type_of_x, x)   cilk_return_no_prof(x)
#define cilk_void_return      cilk_void_return_no_prof
#define spawn_(spawn_stmt)              spawn_no_prof(spawn_stmt)
#define spawn__(spawn_stmt, file, line) spawn_no_prof(spawn_stmt)
#define cilk_sync_            sync_no_prof
#define _Cilk_sync_           sync_no_prof
#define sync_                 sync_no_prof
#define sync__(file, line)    sync_no_prof

#endif
