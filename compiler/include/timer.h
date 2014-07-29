#ifndef _timer_h_
#define _timer_h_

#include <sys/time.h>

class Timer {
public:
                 Timer();
                ~Timer();

  void           clear();

  void           start();
  void           stop();

  double         elapsedSecs()  const;
  unsigned long  elapsedUsecs() const;

private:
  unsigned long  diffUsec()     const;

  struct timeval mRefTime;

  bool           mRunning;
  unsigned long  mAccumUsec;
};

#endif
