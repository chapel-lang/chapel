#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>		       // for malloc()
#include <stdio.h>
#include "maestro_sched.h"

static int maestro_sched_init = 0;
static int * allowed_workers;

static void ms_init(void);
static void ms_init(){
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  int workers = qthread_num_workers();
#else
  int workers = qthread_num_shepherds();
#endif
  allowed_workers = (int*) malloc(workers*sizeof(int));
}

void maestro_sched(enum trigger_type type, enum trigger_action action, int val){

  if (!maestro_sched_init) ms_init();
    switch (action){
      case(MTA_LOWER_STREAM_COUNT):
	{
	  allowed_workers[val] = maestro_current_workers(val)/2;
	  break;
	}
      case(MTA_RAISE_STREAM_COUNT):
	{
	  allowed_workers[val] = maestro_allowed_workers();
	  break;
	} 
      case (MTA_OTHER):
      default: break;
    }
    

};

int maestro_allowed_workers() {

  int sheps = qthread_num_shepherds();
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  int workers = qthread_num_workers();
  int parallelWidth = workers/sheps;
  // even to max count if threads will be uneven on shepherds
  if ((parallelWidth * sheps) != workers) parallelWidth += 1;
#else
    int parallelWidth = sheps();
#endif
  if (!maestro_sched_init) ms_init();
    if (allowed_workers[0] == 0) {
      int i;
      for (i = 0; i < sheps; i ++) {
	allowed_workers[i] = parallelWidth;
      }
    }
    return parallelWidth;
}

int maestro_current_workers(int core_num) {  // need way to allow different worker count for different loops

    return allowed_workers[core_num];   
}

