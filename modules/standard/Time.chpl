_extern def chpl_init_timer(timer);
_extern def chpl_now_timer(timer): _timervalue;
_extern def chpl_seconds_timer(timer): real;
_extern def chpl_microseconds_timer(timer): real;
_extern def chpl_now_year(): int;
_extern def chpl_now_month(): int;
_extern def chpl_now_day(): int;
_extern def chpl_now_dow(): int;
_extern def chpl_now_time(): real;

enum TimeUnits { microseconds, milliseconds, seconds, minutes, hours };
enum Day { sunday=0, monday, tuesday, wednesday, thursday, friday, saturday };

// returns time elapsed since midnight
def getCurrentTime(unit: TimeUnits = TimeUnits.seconds)
  return _convert_microseconds(unit, chpl_now_time());

// returns (year, month, day) as a tuple of 3 ints
def getCurrentDate()
  return (chpl_now_year()+1900,
          chpl_now_month()+1,
          chpl_now_day());

def getCurrentDayOfWeek() {
    return chpl_now_dow():Day;
}

record Timer {
  var time: _timervalue;
  var accumulated: real = 0.0;
  var running: bool = false;

  def initialize() {
    chpl_init_timer(time);
  }

  def clear() {
    accumulated = 0.0;
    running = false;
  }

  def start() {
    if !running {
      running = true;
      time = chpl_now_timer(time);
    } else {
      halt("start called on a timer that has not been stopped");
    }
  }

  def stop() {
    if running {
      var time2: _timervalue;
      time2 = chpl_now_timer(time2);
      accumulated += _diff_timer(time2, time);
      running = false;
    } else {
      halt("stop called on a timer that has not been started");
    }
  }

  def elapsed(unit: TimeUnits = TimeUnits.seconds) {
    if running {
      var time2: _timervalue;
      time2 = chpl_now_timer(time2);
      return _convert_microseconds(unit, accumulated + _diff_timer(time2, time));
    } else {
      return _convert_microseconds(unit, accumulated);
    }
  }
}

// returns diff of two timer values in microseconds
pragma "inline"
def _diff_timer(t1: _timervalue, t2: _timervalue) {
  var s1 = chpl_seconds_timer(t1);
  var s2 = chpl_seconds_timer(t2);
  var us1 = chpl_microseconds_timer(t1);
  var us2 = chpl_microseconds_timer(t2);
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
  __primitive("task sleep", t);
}
