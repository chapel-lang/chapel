enum TimeUnits { microseconds, milliseconds, seconds, minutes, hours };

// returns time elapsed since midnight
def getCurrentTime(unit: TimeUnits) {
  halt("getCurrentTime() is not implemented yet");
  return 0.0;
}

// returns (year, month, day) as a tuple of 3 ints
def getCurrentDate() {
  halt("getCurrentDate() is not implemented yet");
  return (0, 0, 0);
}

record Timer {
  var time: _timervalue;
  var accumulated: float = 0.0;
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
      accumulated = _diff_timer(time2, time);
      running = false;
    } else {
      halt("stop called on a timer that has not been started");
    }
  }

  def elapsed(unit: TimeUnits = seconds) {
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
def _convert_microseconds(unit: TimeUnits, us: float) {
  select unit {
    when microseconds do return us;
    when milliseconds do return us / 1.0e+3;
    when seconds do return us / 1.0e+6;
    when minutes do return us / 60.0e+6;
    when hours do return us / 360.0e+6;
  }
  halt("internal error in module Time");
}
