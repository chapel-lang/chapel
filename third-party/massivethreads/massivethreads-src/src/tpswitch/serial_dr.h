/* 
 * Serial + dag_recorder
 */

/* 
   this file provides macros with which users can
   easily turn on/off dag recorder for your serial
   programs (of course, there is real no point in 
   using dag recorder for serial programs, but it
   is still useful for analyzing programs work,
   critical path, etc.)

   provided macros are:

   mk_task_group;
   create_task(statement);
   create_taskc(callable);
   wait_tasks;

   when DAG_RECORDER is set to a number >= 2,
   they insert instrumentation code for dag
   recorder.

 */

#pragma once
#if DAG_RECORDER>=2
#include <dag_recorder.h>
#endif

#define mk_task_group_with_prof  int __mk_task_group_n_children__ __attribute__((unused)) = 0

#define create_task_with_prof(statement) do {			\
    if (__mk_task_group_n_children__ == 0) dr_begin_section(); \
    __mk_task_group_n_children__++;				\
    dr_dag_node * __c__ = 0;				   \
    dr_dag_node * __t__ = dr_enter_create_task(&__c__);	   \
    dr_start_task(__c__);				   \
    statement;						   \
    dr_end_task();					   \
    dr_return_from_create_task(__t__);			   \
  } while(0)

#define create_task_with_prof_(statement, file, line) do {              \
    if (__mk_task_group_n_children__ == 0) dr_begin_section();          \
    __mk_task_group_n_children__++;                                     \
    dr_dag_node * __c__ = 0;                                            \
    dr_dag_node * __t__ = dr_enter_create_task_(&__c__, file, line);    \
    dr_start_task_(__c__, file, line);                                  \
    statement;                                                          \
    dr_end_task_(file, line);                                           \
    dr_return_from_create_task_(__t__, file, line);                     \
  } while(0)

#define create_taskc_with_prof(callable) create_task_with_prof(callable())

#define wait_tasks_with_prof do {			\
  if (__mk_task_group_n_children__ == 0) dr_begin_section(); \
  dr_return_from_wait_tasks(dr_enter_wait_tasks());	\
  __mk_task_group_n_children__ = 0;			\
} while(0)

#define wait_tasks_with_prof_(file,line) do {			  \
    if (__mk_task_group_n_children__ == 0) dr_begin_section();    \
    dr_return_from_wait_tasks_(dr_enter_wait_tasks_(file, line), file, line); \
    __mk_task_group_n_children__ = 0;                             \
  } while(0)

#if DAG_RECORDER>=2

#define mk_task_group   mk_task_group_with_prof
#define create_task(s)  create_task_with_prof(s)
#define create_task_(s, file, line)  create_task_with_prof_(s, file, line)
#define create_taskc(c) create_taskc_with_prof(c)
#define wait_tasks      wait_tasks_with_prof
#define wait_tasks_(file, line) wait_tasks_with_prof_(file, line)
#define dr_get_max_workers() 1
#define dr_get_worker()      0

#else

#define mk_task_group   int __mk_task_group_n_children__ __attribute__((unused)) = 0
#define create_task(s)  s
#define create_task_(s, file, line)  s
#define create_taskc(c) c()
#define wait_tasks      
#define wait_tasks_(file, line) 

#endif

