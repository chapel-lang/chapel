#ifndef _timer_h_
#define _timer_h_

#include <cstdio>
#include <sys/time.h>

typedef struct timeval _timervalue;

class Timer {
public:
  _timervalue time;
  double accumulated;
  bool running;

  Timer();
  virtual ~Timer() { }

  void clear();
  void start();
  void stop();
  double elapsed();
  void print(FILE* f);
};

#endif
