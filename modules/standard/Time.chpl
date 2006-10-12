record Timer {
  var time: _timervalue;
  var elapsed: float = 0.0;
  var running: bool = false;

  def initialize() {
    __primitive("_init_timer", time);
  }

  def clear() {
    elapsed = 0.0;
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
      elapsed = __primitive("_diff_timer", time2, time);
      running = false;
    } else {
      halt("stop called on a timer that has not been started");
    }
  }

  def value {
    if running {
      var time2: _timervalue;
      time2 = __primitive("_now_timer", time2);
      return elapsed + __primitive("_diff_timer", time2, time);
    } else {
      return elapsed;
    }
  }
}
