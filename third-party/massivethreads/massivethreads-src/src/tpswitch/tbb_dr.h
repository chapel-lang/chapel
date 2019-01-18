/* 
 * tbb_dr.h
 */
/* 
   this file provides macros with which users can
   easily turn on/off dag recorder for your 
   TBB-like task parallel programs.

   all functions are actually provided 
   by MassiveThreads's mtbb/task_group.h
   class, which has the following interfaces.

   mtbb::task_group tg;
   tg.run(callable);
   tg.wait();

   when C++ compiler support lambda expressions,
   you can pass a lambda expression to run().
   e.g.,
   tg.run([=,&x] { x = foo(); });

   when DAG_RECORDER is set to a number >= 2,
   they insert instrumentation code for dag
   recorder.

 */

#pragma once
#include <mtbb/task_group.h>
