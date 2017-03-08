#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>		       // for malloc()
#include <stdio.h>
#include "qthread_innards.h"           // for qthread_num_shepherds/qthread_num_workers
#include "maestro_sched.h"

void saveEnergy(int64_t i);
void resetEnergy(int64_t i);

static int maestro_sched_init = 0;
volatile int * allowed_workers = NULL;

//MUCH of the file MODIFIED for power throttling akp

int64_t currentThreads = 0;
int64_t preferredThreads = 0;
int64_t preferredThreadsPerShep = 0;

static void ms_init(void){
  int i;
  maestro_sched_init = 1;
  int sheps = qthread_num_shepherds();
  int workers = qthread_num_workers();
  int activeWorkers = qthread_readstate(ACTIVE_WORKERS);
  if (workers > activeWorkers) workers = activeWorkers;
  int parallelWidth = workers/sheps;
  // even to max count if threads will be uneven on shepherds
  if ((parallelWidth * sheps) != workers) parallelWidth += 1;

  allowed_workers = (int*) qt_malloc(sheps * sizeof(int));
  for ( i = 0; i < sheps; i ++) {
    allowed_workers[i] = parallelWidth;
  }
  preferredThreads = workers;
  currentThreads = workers;
  for ( i = 0; i < workers; i ++) {
    //    resetEnergy(i);
  }
}

// NEED REAL MODEL HERE (maybe can get one with collaboration with UDelaware - John Cavazos)

int64_t maestro_size(void){
  if (!maestro_sched_init) ms_init();
  return currentThreads;
}

int powerOff = 0;
int threadsPowerActivePerSocket;
int omp_in_parallel(void);

qt_barrier_t * sinc_barrier = NULL;

int64_t maestro_change_size(void){
  int i;
  if (omp_in_parallel()) return currentThreads; 
    // if in nested parallel freeze size because I'm not sure the best way to shrink/expand the barriers
    // thoughout the nested stack (including sibling regions) akp 10/12/12
  if (!maestro_sched_init) ms_init();
  if (currentThreads != preferredThreads) {
    // printf("\nchange size old %ld  new %ld\n", currentThreads, preferredThreads);
    qt_barrier_t *currentBarrier =  qt_get_barrier();
    if (currentBarrier) qt_barrier_resize(currentBarrier, preferredThreads);
    int sheps = qthread_num_shepherds();
    for ( i = 0; i < sheps; i ++) {
      allowed_workers[i] = preferredThreadsPerShep;
    }
    if (preferredThreads > currentThreads) { // turn cores back to speed
      int j;
      powerOff = 0;
      threadsPowerActivePerSocket = preferredThreadsPerShep;
    }
    else { // slow cores down that aren't doing really available
      int j;
      powerOff = 1;
      threadsPowerActivePerSocket = preferredThreadsPerShep;
      saveEnergy(qthread_num_workers()-1);
    }
    currentThreads = preferredThreads;
  }
  return currentThreads;
}

void maestro_sched(enum trigger_type type, enum trigger_action action, int val){
  int i;
  if (!maestro_sched_init) ms_init();
  int sheps = qthread_num_shepherds(); 
  switch (action){
  case(MTA_LOWER_STREAM_COUNT):
    {
      preferredThreadsPerShep = (maestro_allowed_workers()*3)/4;
      preferredThreads = preferredThreadsPerShep*sheps;
      //     printf("lower thread count\n");
      break;
    }
  case(MTA_RAISE_STREAM_COUNT):
    {
      preferredThreadsPerShep = maestro_allowed_workers();
      preferredThreads = (preferredThreadsPerShep*sheps)-1;
      //printf("raise thread count\n");
      break;
    } 
  case (MTA_OTHER):
  default: break;
  }
};

int maestro_allowed_workers() {

  if (!maestro_sched_init) ms_init();
  int sheps = qthread_num_shepherds();
  int workers = qthread_num_workers();
  int parallelWidth = workers/sheps;
  // even to max count if threads will be uneven on shepherds
  if ((parallelWidth * sheps) != workers) parallelWidth += 1;
  return parallelWidth;
}

int maestro_current_workers(int core_num) {  // need way to allow different worker count for different loops

    return allowed_workers[core_num];   
}

#if 0
int rcrToolContinue = 1;

void doWork(int a,int b) {
 
    struct timespec freq, remainder;

    freq.tv_nsec = 1000000;
    freq.tv_sec = 0;

    printf("\nRCRTool: Qthreads built without RCRdaemon access -- no resource statistics or throttling\n");

    while (rcrToolContinue) {
      if (freq.tv_sec != 0 || freq.tv_nsec != 0)
     	nanosleep(&freq, &remainder);
    }
}

#endif

#if defined(__x86_64__)
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS
#include <sys/types.h>

#define IA32_CLOCK_MODULATION           0x19A

int64_t msrNotAvailable = 0;

int MSRWrite(int cpu, uint32_t reg, uint64_t data)
{
  int fd;
  int c;
  unsigned long arg;
  char *endarg;
  char msr_file_name[64];
  
  if (msrNotAvailable) return -1;
  
  sprintf(msr_file_name, "/dev/cpu/%d/msr", cpu);
  fd = open(msr_file_name, O_WRONLY);
  if (fd < 0) { 
    if (errno == ENXIO) {
      fprintf(stderr, "RCRTool: No CPU %d\n", cpu);
      return -1;
    } else if (errno == EIO) {
      fprintf(stderr, "RCRTool: CPU %d doesn't support MSRs\n",
	      cpu);
      return -1;
    } else {
      perror("RCRTool: ");
      return -1;
    }
  }
  
  if (pwrite(fd, &data, sizeof data, reg) != sizeof data) {
    if (errno == EIO) {
      fprintf(stderr,
	      "wrmsr: CPU %d cannot set MSR "
	      "0x%08"PRIx32" to 0x%016"PRIx64"\n",
	      cpu, reg, data);
      return(4);
    } else {
            perror("wrmsr: pwrite");
            return(127);
    }
  }
  
  close(fd);
  
  return(0);
}

void saveEnergy(int64_t i){
  MSRWrite(i, IA32_CLOCK_MODULATION, 0x11); 
}

void resetEnergy(int64_t i){
  MSRWrite(i, IA32_CLOCK_MODULATION, 0x00);
}

#endif
