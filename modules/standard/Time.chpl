/*
 * Copyright 2004-2015 Cray Inc.
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




/*
   This module provides support for querying wall time in the local
   timezone and implements a record :record:`~Timer` that provides basic
   stopwatch behavior.  The stopwatch has the potential for microsecond
   resolution and is intended to be useful for performance testing.
 */

module Time {

pragma "no doc"
// Returns the number of seconds since midnight.  Has the potential for
// microsecond resolution if supported by the runtime platform
private extern proc chpl_now_time():real;




pragma "no doc"
// This is comparable to a Posix struct timeval
extern type _timevalue;



pragma "no doc"
private extern proc chpl_null_timevalue(): _timevalue;



pragma "no doc"
// The number of seconds/microseconds since Jan 1, 1970 in UTC
private extern proc chpl_now_timevalue():  _timevalue;



pragma "no doc"
// The components of time in the local time zone
private extern proc chpl_timevalue_parts(t:           _timevalue,

                                         out seconds: int(32),
                                         out minutes: int(32),
                                         out hours:   int(32),
                                         out mday:    int(32),
                                         out month:   int(32),
                                         out year:    int(32),
                                         out wday:    int(32),
                                         out yday:    int(32),
                                         out isdst:   int(32));

/* Specifies the units to be used when certain functions return a time */
enum TimeUnits { microseconds, milliseconds, seconds, minutes, hours }

/* Specifies the day of the week */
enum Day       { sunday=0, monday, tuesday, wednesday, thursday, friday, saturday }

/*
   :arg  unit: The units for the returned value
   :type unit: TimeUnits

   :returns: The elapsed time since midnight, local time, in the units specified
   :rtype:   real(64)
 */
proc getCurrentTime(unit: TimeUnits = TimeUnits.seconds) : real(64)
  return _convert_microseconds(unit, chpl_now_time());

/*
   :returns:  (year, month, day) as a tuple of 3 ints

   The month is in the range 1 to 12.
   The day   is in the range 1 to 31
*/
proc getCurrentDate() {
  var now = chpl_now_timevalue();

  var seconds, minutes, hours, mday, month, year, wday, yday, isdst:int(32);

  chpl_timevalue_parts(now, seconds, minutes, hours, mday, month, year, wday, yday, isdst);

  return (year + 1900, month + 1, mday);
}

/*
   :returns: The current day of the week
   :rtype:   Day
 */
proc getCurrentDayOfWeek() : Day {
  var now = chpl_now_timevalue();

  var seconds, minutes, hours, mday, month, year, wday, yday, isdst:int(32);

  chpl_timevalue_parts(now, seconds, minutes, hours, mday, month, year, wday, yday, isdst);

  return wday : Day;
}

/* Delay a task for t seconds */
inline proc sleep(t: uint) : void {
  extern proc chpl_task_sleep(t:uint) : void;

  chpl_task_sleep(t);
}


/*
   Implements basic stopwatch behavior with a potential resolution of
   microseconds if supported by the runtime platform.

   The :record:`!Timer` can be started, stopped, and cleared.
   A :record:`!Timer` is either running or stopped.
*/

record Timer {
  pragma "no doc"
  var time:        _timevalue = chpl_null_timevalue();

  pragma "no doc"
  var accumulated: real       = 0.0;

  pragma "no doc"
  var running:     bool       = false;

  pragma "no doc"
  proc initialize() {
    // does nothing.
  }

  /*
     Clears the elapsed time. If the timer is running then it is restarted
     otherwise it remains in the stopped state.
  */
  proc clear() : void {
    accumulated = 0.0;

    if running {
      time = chpl_now_timevalue();
    }
  }

  /* Starts the timer. It is an error to start a timer that is already running. */
  proc start() : void {
    if !running {
      running = true;
      time    = chpl_now_timevalue();
    } else {
      halt("start called on a timer that has not been stopped");
    }
  }

  /* Stops the timer. It is an error to stop a timer that is not running. */
  proc stop() : void {
    if running {
      var time2: _timevalue = chpl_now_timevalue();

      accumulated += _diff_time(time2, time);
      running      = false;
    } else {
      halt("stop called on a timer that has not been started");
    }
  }

  /*
     Returns the cumulative elapsed time, in the units specified, between
     all pairs of calls to :proc:`~!Timer.start` and :proc:`~!Timer.stop`
     since the timer was created or the last call to :proc:`~!Timer.clear`.
     If the timer is running, the elapsed time since the last call to
     :proc:`~!Timer.start` is added to the return value.

     :arg  unit: The units for the returned value
     :type unit: TimeUnits

     :returns: The elapsed time in the units specified
     :rtype:   real(64)
  */
  proc elapsed(unit: TimeUnits = TimeUnits.seconds) : real {
    if running {
      var time2: _timevalue = chpl_now_timevalue();

      return _convert_microseconds(unit, accumulated + _diff_time(time2, time));
    } else {
      return _convert_microseconds(unit, accumulated);
    }
  }
}

// returns diff of two time values in microseconds
pragma "no doc"
private inline proc _diff_time(t1: _timevalue, t2: _timevalue) {
  extern proc chpl_timevalue_seconds(t:_timevalue):      int(64);
  extern proc chpl_timevalue_microseconds(t:_timevalue): int(64);

  var s1  = chpl_timevalue_seconds(t1);
  var us1 = chpl_timevalue_microseconds(t1);

  var s2  = chpl_timevalue_seconds(t2);
  var us2 = chpl_timevalue_microseconds(t2);

  return (s1 * 1.0e+6 + us1) - (s2 * 1.0e+6 + us2);
}

// converts microseconds to another unit
pragma "no doc"
private proc _convert_microseconds(unit: TimeUnits, us: real) {
  select unit {
    when TimeUnits.microseconds do return us;
    when TimeUnits.milliseconds do return us /    1.0e+3;
    when TimeUnits.seconds      do return us /    1.0e+6;
    when TimeUnits.minutes      do return us /   60.0e+6;
    when TimeUnits.hours        do return us / 3600.0e+6;
  }

  halt("internal error in module Time");

  // will never get here, but to avoid warnings:
  return -1.0;
}

}
