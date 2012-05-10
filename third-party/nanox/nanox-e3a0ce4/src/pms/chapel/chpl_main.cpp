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

#include <assert.h>

#include "system.hpp"
#include "basethread.hpp"
#include "schedule.hpp"
#include "smpdd.hpp"
#include "chpl_nanos.h"
#include "smpdd.hpp"
#include "chpl_pmdata.hpp"

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
using namespace nanos::chapel;

static bool chapel_hooked = false;

void nanos_chapel_pre_init ( void * dummy )
{
   sys.setDelayedStart(true);
   sys.setPMInterface( NEW nanos::chapel::ChapelPMInterface() );
   chapel_hooked = true;
}

//
// Used for debugging purposes only
//
static int32_t chpl_localeID = -1;

#define PRINTF_DEF printf

#ifndef BRAD_DEBUGGING
#define NANOX_SANITY_CHECK(routine)
#else
#define NANOX_SANITY_CHECK(routine) \
  if (nanos::myThread == NULL) { fprintf(stderr, "[%d:%ld] non-Nanox thread detected in chpl_task_%s()\n", chpl_localeID, pthread_self(), #routine); } \
  else { fprintf(stderr, "[%d:%d:%ld] thread in chpl_task_%s()\n", chpl_localeID, (nanos::myThread)->getId(), pthread_self(), #routine); }
#endif


void * chpl_smp_factory( void *prealloc, void *args );
void * chpl_smp_factory( void *prealloc, void *args )
{
   chpl_fn_p outline = ( chpl_fn_p ) args;

   if ( prealloc != NULL ) {
     return ( void * )new (prealloc) SMPDD( outline );
   } else {
     return ( void * )new SMPDD( outline );
   }
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
  NANOX_SANITY_CHECK(begin);
   assert(!ltask);

   if ( !ignore_serial && chpl_task_getSerial() ) {	   
      (*fp)(a);
      return;
   }
   
   WD * wd = 0;
   nanos_device_t devs[] = {{ chpl_smp_factory , sizeof(SMPDD), (void *) fp}};
   sys.createWD(&wd, 1, devs, 0, 1, &a, NULL, NULL, 0, NULL, NULL);

   ChapelWDData *data = (ChapelWDData *)wd->getInternalData();
   data->serial = serial_state;

   sys.submit(*wd);
}

// Tasks

void nanos_chpl_task_init(int32_t numThreadsPerLocale, 
                          int32_t maxThreadsPerLocale, int numCommTasks, 
                          uint64_t callStackSize, 
                          int32_t init_chpl_localeID) {
   NANOX_SANITY_CHECK(init);

   fatal_cond0(!chapel_hooked, "Chapel layer has not been correctly initialized");

   if (numThreadsPerLocale == 0) {
     numThreadsPerLocale = chpl_numCoresOnThisLocale() + numCommTasks;
   }

   sys.setInitialMode( System::POOL );
   sys.setUntieMaster(true);
   sys.setNumPEs(numThreadsPerLocale);
   sys.start();

   NANOX_SANITY_CHECK(init);

   //
   // TODO: Should verify that callStackSize is a reasonable value and
   // use it to set Nanos++'s task callstack size.
   //
   taskCallStackSize = callStackSize;

   chpl_localeID = init_chpl_localeID;

/*  tp = chpl_mem_alloc(sizeof(thread_private_data_t), CHPL_RT_MD_THREAD_PRIVATE_DATA, 0, 0);
  threadlayer_set_thread_private_data(tp);
  tp->serial_state = false;*/
}


void chpl_task_callMain(void (*chpl_main)(void)) {
  NANOX_SANITY_CHECK(callMain);
  chpl_main();
}

void chpl_task_exit()
{
  NANOX_SANITY_CHECK(exit);
   sys.finish();
}

void chpl_task_addToTaskList(chpl_fn_int_t fid, void* arg,
                             chpl_task_list_p *task_list,
                             int32_t task_list_locale,
                             chpl_bool call_chpl_begin,
                             int lineno, chpl_string filename) {
  NANOX_SANITY_CHECK(addToTaskList);
    chpl_fn_p fp = chpl_ftable[fid];
    chpl_task_begin(fp, arg, false, false, NULL);
}

void chpl_task_processTaskList(chpl_task_list_p task_list)
{
  NANOX_SANITY_CHECK(processTaskList);
}

void chpl_task_executeTasksInList(chpl_task_list_p task_list)
{
  NANOX_SANITY_CHECK(executeTasksInList);
}

void chpl_task_freeTaskList(chpl_task_list_p task_list)
{
  NANOX_SANITY_CHECK(freeTaskList);
}

//TODO
void chpl_task_sleep(int secs)
{
  NANOX_SANITY_CHECK(sleep);
  sleep(secs);
}

chpl_bool chpl_task_getSerial(void)
{
  NANOX_SANITY_CHECK(getSerial);
  WD* wd = myThread->getCurrentWD();
  ChapelWDData* data = (ChapelWDData*)wd->getInternalData();
  return data->serial;
}

void chpl_task_setSerial(chpl_bool state)
{
  NANOX_SANITY_CHECK(setSerial);
  WD* wd = myThread->getCurrentWD();
  ChapelWDData* data = (ChapelWDData*)wd->getInternalData();
  data->serial = state;
}

uint64_t chpl_task_getCallStackSize(void) {
  NANOX_SANITY_CHECK(getCallStackSize);
  return taskCallStackSize;
}


// Task stats routines

uint32_t chpl_task_getNumQueuedTasks(void)
{
  NANOX_SANITY_CHECK(getNumQueuedTasks);
   return sys.getReadyNum();
}

uint32_t chpl_task_getNumRunningTasks(void)
{
  NANOX_SANITY_CHECK(getNumRunningTasks);
  return sys.getRunningTasks();
}

int32_t  chpl_task_getNumBlockedTasks(void)
{
  NANOX_SANITY_CHECK(getNumBlockedTasks);
  return sys.getTaskNum() - sys.getReadyNum() -  sys.getRunningTasks();
}

//TODO
chpl_taskID_t chpl_task_getId(void)
{
  NANOX_SANITY_CHECK(getId);
  return myThread->getCurrentWD()->getId();
}

// Threads stat routines

uint32_t chpl_task_getNumThreads(void)
{
  NANOX_SANITY_CHECK(getNumThreads);
   return sys.getNumWorkers();
}

uint32_t chpl_task_getNumIdleThreads(void)
{
  NANOX_SANITY_CHECK(getNumIdleThreads);
    return sys.getIdleNum();
}

int chpl_task_createCommTask(chpl_fn_p fn, void* arg) {
  NANOX_SANITY_CHECK(createCommTask);

  // TODO: Eventually, we will want to make this task more dedicated
  // than your average Nanos task, but for now we believe this should
  // work as long as the number of threads Nanos is using is > 1.
  chpl_task_begin(fn, arg, true, false, NULL);
  return 0;
}

void chpl_task_yield(void) {
  Scheduler::yield();
}
