#include "timer.h"
#include "misc.h"

static void
_now_timer(_timervalue* time) {
  struct timezone tz;
  gettimeofday(time, &tz);
}

static double
_diff_timer(_timervalue* time1, _timervalue* time2) {
  double s1 = (double)(time1->tv_sec);
  double s2 = (double)(time2->tv_sec);
  double us1 = (double)(time1->tv_usec);
  double us2 = (double)(time2->tv_usec);
  return (s1*1.0e+6+us1)-(s2*1.0e+6+us2);
}

Timer::Timer() :
  time(),
  accumulated(0.0),
  running(false)
{}

void Timer::clear() {
  accumulated = 0.0;
  running = false;
}

void Timer::start() {
  if (!running) {
    running = true;
    _now_timer(&time);
  } else {
    INT_FATAL("start called on a timer that has not been stopped");
  }
}

void Timer::stop() {
  if (running) {
    _timervalue time2;
    _now_timer(&time2);
    accumulated += _diff_timer(&time2, &time);
    running = false;
  } else {
    INT_FATAL("stop called on a timer that has not been started");
  }
}

double Timer::elapsed() {
  if (running) {
    _timervalue time2;
    _now_timer(&time2);
    return (accumulated + _diff_timer(&time2, &time)) / 1.0e+6;
  } else {
    return accumulated / 1.0e+6;
  }
}

void Timer::print(FILE* f) {
  fprintf(f, "%0.3lf", elapsed());
}
