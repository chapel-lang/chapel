/*************************************************************************************/
/*      Copyright 2010 Barcelona Supercomputing Center                               */
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

#include "chpl_nanos.h"
#include <assert.h>

#include "system.hpp"
#include "basethread.hpp"
#include "schedule.hpp"
#include "smpdd.hpp"

// // TODO: include chpl headers?
// typedef char * chpl_string;
// typedef bool chpl_bool;
// typedef void (*chpl_fn_p) (void *);
// typedef void * chpl_task_list_p;
// typedef int chpl_fn_int_t;
// typedef int chpl_taskID_t;

extern chpl_fn_p chpl_ftable[];

static uint64_t taskCallStackSize;

using namespace nanos;
using namespace nanos::ext;

static bool chapel_hooked = false;

void nanos_chapel_pre_init ( void * dummy )
{
   sys.setDelayedStart(true);
   chapel_hooked = true;
}

//
// interface function with begin-statement
//
void chpl_task_begin(chpl_fn_p fp,
                     void* a,
                     chpl_bool ignore_serial,  // always add task to pool
                     chpl_bool serial_state,
                     chpl_task_list_p ltask) 
{

   assert(!ltask);

   WD * wd = NEW WD( NEW SMPDD( fp ), 0, 0, a );
   sys.submit(*wd);
}

// Tasks

void nanos_chpl_task_init(int32_t maxThreadsPerLocale, uint64_t callStackSize) {
   fatal_cond0(!chapel_hooked, "Chapel layer has not been correctly initialized");

   sys.setInitialMode( System::POOL );
   sys.setUntieMaster(true);
   sys.setNumPEs(maxThreadsPerLocale);
   sys.start();

   //
   // TODO: Should verify that callStackSize is a reasonable value and
   // use it to set Nanos++'s task callstack size.
   //
   taskCallStackSize = callStackSize;

/*  tp = chpl_alloc(sizeof(thread_private_data_t), CHPL_RT_MD_THREAD_PRIVATE_DATA, 0, 0);
  threadlayer_set_thread_private_data(tp);
  tp->serial_state = false;*/
}


void chpl_task_callMain(void (*chpl_main)(void)) {
  chpl_main();
}

void chpl_task_exit()
{
   sys.finish();
}

void chpl_task_addToTaskList(chpl_fn_int_t fid, void* arg,
                             chpl_task_list_p *task_list,
                             int32_t task_list_locale,
                             chpl_bool call_chpl_begin,
                             int lineno, chpl_string filename) {
    chpl_fn_p fp = chpl_ftable[fid];
    chpl_task_begin(fp, arg, false, false, NULL);
}

void chpl_task_processTaskList(chpl_task_list_p task_list)
{
}

void chpl_task_executeTasksInList(chpl_task_list_p task_list)
{
}

void chpl_task_freeTaskList(chpl_task_list_p task_list)
{
}

//TODO
void chpl_task_sleep(int secs)
{
  sleep(secs);
}

//TODO
chpl_bool chpl_task_getSerial(void)
{
  return 0;
}

//TODO
void chpl_task_setSerial(chpl_bool state)
{
}

uint64_t chpl_task_getCallStackSize(void) {
  return taskCallStackSize;
}


// Task stats routines

uint32_t chpl_task_getNumQueuedTasks(void)
{
   return sys.getReadyNum();
}

uint32_t chpl_task_getNumRunningTasks(void)
{
  return sys.getRunningTasks();
}

int32_t  chpl_task_getNumBlockedTasks(void)
{
  return sys.getTaskNum() - sys.getReadyNum() -  sys.getRunningTasks();
}

//TODO
chpl_taskID_t chpl_task_getId(void)
{
  return myThread->getCurrentWD()->getId();
}

// Threads stat routines

int32_t  chpl_task_getMaxThreads(void)
{
   // TODO: Alex
   return 0;
}

int32_t  chpl_task_getMaxThreadsLimit(void)
{
   // TODO: Alex
   return 0;
}

uint32_t chpl_task_getNumThreads(void)
{
   return sys.getNumWorkers();
}

uint32_t chpl_task_getNumIdleThreads(void)
{
    return sys.getIdleNum();
}

void chpl_task_perPthreadInit(void) {
  // TODO: This function is called for any pthreads that are created
  // outside the tasking layer -- for example, a progress thread used
  // to ensure progress in a one-sided communication layer that needs
  // software support.  If anything needs to be done with these
  // threads to help them coexist with those owned by the tasking
  // layer, it should be done here.
}
