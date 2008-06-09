enum TimeUnits { microseconds, milliseconds, seconds, minutes, hours };
enum Day { sunday, monday, tuesday, wednesday, thursday, friday, saturday };

// returns time elapsed since midnight
def getCurrentTime(unit: TimeUnits = TimeUnits.seconds)
  return _convert_microseconds(unit, __primitive("_now_time"));

// returns (year, month, day) as a tuple of 3 ints
def getCurrentDate()
  return (__primitive("_now_year")+1900,
          __primitive("_now_month")+1,
          __primitive("_now_day"));

def getCurrentDayOfWeek() {
    return (__primitive("_now_dow")+1):Day;
}

record Timer {
  var time: _timervalue;
  var accumulated: real = 0.0;
  var running: bool = false;

  def initialize() {
    __primitive("_init_timer", time);
  }

  def clear() {
    accumulated = 0.0;
    running = false;
  }

  def start() {
    if !running {
      running = true;
      time = __primitive("_now_timer", time);
    } else {
      halt("start called on a timer that has not been stopped");
    }
  }

  def stop() {
    if running {
      var time2: _timervalue;
      time2 = __primitive("_now_timer", time2);
      accumulated += _diff_timer(time2, time);
      running = false;
    } else {
      halt("stop called on a timer that has not been started");
    }
  }

  def elapsed(unit: TimeUnits = TimeUnits.seconds) {
    if running {
      var time2: _timervalue;
      time2 = __primitive("_now_timer", time2);
      return _convert_microseconds(unit, accumulated + _diff_timer(time2, time));
    } else {
      return _convert_microseconds(unit, accumulated);
    }
  }
}

// returns diff of two timer values in microseconds
pragma "inline"
def _diff_timer(t1: _timervalue, t2: _timervalue) {
  var s1 = __primitive("_seconds_timer", t1);
  var s2 = __primitive("_seconds_timer", t2);
  var us1 = __primitive("_microseconds_timer", t1);
  var us2 = __primitive("_microseconds_timer", t2);
  return (s1*1.0e+6+us1)-(s2*1.0e+6+us2);
}

// converts microseconds to another unit
def _convert_microseconds(unit: TimeUnits, us: real) {
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

pragma "inline" def sleep(t: uint) {
  __primitive("sleep", t);
}
