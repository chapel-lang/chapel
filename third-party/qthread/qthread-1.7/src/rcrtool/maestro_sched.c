#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>		       // for malloc()
#include <stdio.h>
#include "qthread_innards.h"           // for qthread_num_shepherds/qthread_num_workers
#include "maestro_sched.h"

static int maestro_sched_init = 0;
static int * allowed_workers = NULL;

static void ms_init(void);

static void ms_init(void){
  int i;
  maestro_sched_init = 1;
  int sheps = qthread_num_shepherds();
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  int workers = qthread_num_workers();
  int parallelWidth = workers/sheps;
  // even to max count if threads will be uneven on shepherds
  if ((parallelWidth * sheps) != workers) parallelWidth += 1;
#else
  int parallelWidth = 1;  // so hardwire width to 1
#endif

  allowed_workers = (int*) malloc(sheps*sizeof(int));
  for ( i = 0; i < sheps; i ++) {
    allowed_workers[i] = parallelWidth;
  }
}

// NEED REAL MODEL HERE (maybe can get one with collaboration with UDelaware - John Cavazos)

void maestro_sched(enum trigger_type type, enum trigger_action action, int val){
  int i;
  if (!maestro_sched_init) ms_init();
  int sheps = qthread_num_shepherds();
  switch (action){
  case(MTA_LOWER_STREAM_COUNT):
    {
      // drop all to 3/4 -- works better on ldmapper2 test 
      for ( i = 0; i < sheps; i ++) {
	allowed_workers[i] = (maestro_allowed_workers()*3)/4;
      }
      // previous model -- drop local to 1/2
      //      allowed_workers[val] = maestro_current_workers(val)/2;
      break;
    }
  case(MTA_RAISE_STREAM_COUNT):
    {
      for ( i = 0; i < sheps; i ++) {
	allowed_workers[i] = maestro_allowed_workers();
      }
      // previous model -- raise local to max
      // allowed_workers[val] = maestro_allowed_workers();
      break;
    } 
  case (MTA_OTHER):
  default: break;
  }
};

int maestro_allowed_workers() {

  if (!maestro_sched_init) ms_init();
  int sheps = qthread_num_shepherds();
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  int workers = qthread_num_workers();
  int parallelWidth = workers/sheps;
  // even to max count if threads will be uneven on shepherds
  if ((parallelWidth * sheps) != workers) parallelWidth += 1;
#else
  int parallelWidth = 1;  // so hardwire width to 1
#endif
  return parallelWidth;
}

int maestro_current_workers(int core_num) {  // need way to allow different worker count for different loops

    return allowed_workers[core_num];   
}

