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

extern type _timevalue;
extern proc chpl_null_timevalue():_timevalue;
extern proc chpl_now_timevalue():_timevalue;
extern proc chpl_timevalue_parts(t:_timevalue, out seconds:int(32), out minutes:int(32), out hours:int(32), out mday:int(32), out month:int(32), out year:int(32), out wday:int(32), out yday:int(32), out isdst:int(32));
extern proc chpl_timevalue_seconds(t:_timevalue):int(64);
extern proc chpl_timevalue_microseconds(t:_timevalue):int(64);
extern proc chpl_now_time():real;

enum TimeUnits { microseconds, milliseconds, seconds, minutes, hours };
enum Day { sunday=0, monday, tuesday, wednesday, thursday, friday, saturday };

// returns time elapsed since midnight
proc getCurrentTime(unit: TimeUnits = TimeUnits.seconds)
  return _convert_microseconds(unit, chpl_now_time());

// returns (year, month, day) as a tuple of 3 ints
proc getCurrentDate() {
  var now = chpl_now_timevalue();
  var seconds, minutes, hours, mday, month, year, wday, yday, isdst:int(32);
  chpl_timevalue_parts(now, seconds, minutes, hours, mday, month, year, wday, yday, isdst);
  return (year+1900,
          month+1,
          mday);
}

proc getCurrentDayOfWeek() {
  var now = chpl_now_timevalue();
  var seconds, minutes, hours, mday, month, year, wday, yday, isdst:int(32);
  chpl_timevalue_parts(now, seconds, minutes, hours, mday, month, year, wday, yday, isdst);
  return wday:Day;
}

record Timer {
  var time: _timevalue = chpl_null_timevalue();
  var accumulated: real = 0.0;
  var running: bool = false;

  proc initialize() {
    // does nothing.
  }

  proc clear() {
    accumulated = 0.0;
    if running {
      time = chpl_now_timevalue();
    }
  }

  proc start() {
    if !running {
      running = true;
      time = chpl_now_timevalue();
    } else {
      halt("start called on a timer that has not been stopped");
    }
  }

  proc stop() {
    if running {
      var time2: _timevalue;
      time2 = chpl_now_timevalue();
      accumulated += _diff_time(time2, time);
      running = false;
    } else {
      halt("stop called on a timer that has not been started");
    }
  }

  proc elapsed(unit: TimeUnits = TimeUnits.seconds) {
    if running {
      var time2: _timevalue;
      time2 = chpl_now_timevalue();
      return _convert_microseconds(unit, accumulated + _diff_time(time2, time));
    } else {
      return _convert_microseconds(unit, accumulated);
    }
  }
}

// returns diff of two time values in microseconds
inline proc _diff_time(t1: _timevalue, t2: _timevalue) {
  var s1 = chpl_timevalue_seconds(t1);
  var s2 = chpl_timevalue_seconds(t2);
  var us1 = chpl_timevalue_microseconds(t1);
  var us2 = chpl_timevalue_microseconds(t2);
  return (s1*1.0e+6+us1)-(s2*1.0e+6+us2);
}

// converts microseconds to another unit
proc _convert_microseconds(unit: TimeUnits, us: real) {
  select unit {
    when TimeUnits.microseconds do return us;
    when TimeUnits.milliseconds do return us / 1.0e+3;
    when TimeUnits.seconds do return us / 1.0e+6;
    when TimeUnits.minutes do return us / 60.0e+6;
    when TimeUnits.hours do return us / 3600.0e+6;
  }
  halt("internal error in module Time");
  // will never get here, but to avoid warnings:
  return -1.0;
}

inline proc sleep(t: uint) {
  extern proc chpl_task_sleep(t:uint) : void;
  chpl_task_sleep(t);
}
