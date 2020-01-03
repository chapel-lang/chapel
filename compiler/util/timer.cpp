/*
 * Copyright 2004-2020 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "timer.h"

#include "misc.h"

Timer::Timer() {
  clear();
}

Timer::~Timer() {

}

void Timer::clear() {
  mRefTime.tv_sec  = 0;
  mRefTime.tv_usec = 0;

  mAccumUsec       = 0;
  mRunning         = false;
}

void Timer::start() {
  if (mRunning == false) {
    gettimeofday(&mRefTime, 0);
    mRunning = true;

  } else {
    INT_FATAL("start called on a timer that has not been stopped");
  }
}

void Timer::stop() {
  if (mRunning == true) {
    mAccumUsec = elapsedUsecs();
    mRunning   = false;

  } else {
    INT_FATAL("stop called on a timer that has not been started");
  }
}

double Timer::elapsedSecs() const {
  return elapsedUsecs() / 1.0e6;
}

unsigned long Timer::elapsedUsecs() const {
  return mAccumUsec + ((mRunning == true) ? diffUsec() : 0);
}

unsigned long Timer::diffUsec() const {
  struct timeval now;

  unsigned long  deltaSec  = 0;
  unsigned long  deltaUsec = 0;

  gettimeofday(&now, 0);

  /* 
     Careful: The arithmetic is based on unsigned longs.

     If now.tv_usec < mRefTime.tv_usec then now.tv_sec > mRefTime.tv.usec
  */

  if (now.tv_usec < mRefTime.tv_usec) {
    deltaSec  = (now.tv_sec  - mRefTime.tv_sec) - 1;
    deltaUsec = (1000000 + now.tv_usec) - mRefTime.tv_usec;

  } else {
    deltaSec  = now.tv_sec  - mRefTime.tv_sec;
    deltaUsec = now.tv_usec - mRefTime.tv_usec;
  }

  return deltaSec * 1000000 + deltaUsec;
}
