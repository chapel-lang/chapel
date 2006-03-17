#ifndef _timer_H_
#define _timer_H_

#include <time.h>

// Note: CLOCK_THREAD_CPUTIME_ID and CLOCK_PROCESS_CPUTIME_ID
// give results for some versions of Linux (e.g. 2.6.11.4-10 SUSE)
// which are quite a bit higher than CLOCK_REALTIME.
// This is highly suspicious, so their use by default is disabled.

#if 0
#ifdef CLOCK_THREAD_CPUTIME_ID
#define TIMER_DEFAULT_CLOCKID_T CLOCK_PROCESS_CPUTIME_ID
#else
#define TIMER_DEFAULT_CLOCKID_T CLOCK_REALTIME
#endif
#else
#define TIMER_DEFAULT_CLOCKID_T CLOCK_REALTIME
#endif

#define TIMER_NUM_ACCUMULATORS 10

class Timer {
 public:
  clockid_t type;
  double start_time;
  double time;
  double accumulator[TIMER_NUM_ACCUMULATORS];

  static double timespec_to_double(struct timespec &ts) {
    return (double)ts.tv_sec + ((double)ts.tv_nsec)/1000000000.0;
  }
  void start() { 
    struct timespec ts; 
    clock_gettime(type, &ts); 
    start_time = timespec_to_double(ts);
  }
  double snap() { 
    struct timespec ts; 
    clock_gettime(type, &ts); 
    double stop_time = timespec_to_double(ts);
    double delta = stop_time - start_time;
    time += delta;
    start_time = stop_time;
    return time;
  }
  double lap() {
    struct timespec ts; 
    clock_gettime(type, &ts); 
    double stop_time = timespec_to_double(ts);
    double delta = stop_time - start_time;
    time += delta;
    start_time = stop_time;
    return delta;
  }
  double accumulate(int i = 0) {
    assert(i < TIMER_NUM_ACCUMULATORS);
    accumulator[i] += time;
    time = 0;
    return accumulator[i];
  }
  void reset() { 
    time = start_time = 0;
  }
  void restart() {
    time = 0;
    start();
  }
  void stop() { snap(); }

  Timer(clockid_t atype = TIMER_DEFAULT_CLOCKID_T) : type(atype) {
    reset();
    start();
  }
};

#endif
