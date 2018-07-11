module Timer {
  use LCALSDataTypes;
  use Time;
  enum TimerType { Clock, Cycle, Chapel };
  config const defaultTimerType: TimerType = TimerType.Chapel;

  class TimerImpl {
    proc start() {
      halt("Called Abstract Base start method");
    }
    proc stop() {
      halt("Called Abstract Base stop method");
    }
    proc elapsed() {
      halt("Called Abstract Base elapsed method");
      return 0.0;
    }
  }

  class ChapelTimer: TimerImpl {
    var t: Timer;
    proc start() {
      t.start();
    }
    proc stop() {
      t.stop();
    }
    proc elapsed() {
      return t.elapsed();
    }
  }

  extern const CLOCKS_PER_SEC: real(64);
  extern proc clock(): uint(64);
  class ClockTimer: TimerImpl {

    var startTime: uint(64);
    var endTime: uint(64);

    proc start() {
      startTime = clock();
    }
    proc stop() {
      endTime = clock();
    }
    proc elapsed() {
      return (endTime - startTime): real(64) / CLOCKS_PER_SEC;
    }
  }

  class CycleTimer: TimerImpl {

    var startTime: uint(64);
    var endTime: uint(64);

    proc init() {
      halt("CycleTimer not implemented");
    }
    proc start() {
      startTime = 0; // = getticks();
    }
    proc stop() {
      endTime = 0; // = getticks();
    }
    proc elapsed() {
      return 0.0;
    }
  }


  class LoopTimer {
    var t: TimerImpl;
    var was_run: bool;

    proc init(timerType: TimerType = defaultTimerType) {
      if timerType == TimerType.Chapel {
        t = new ChapelTimer();
      } else if timerType == TimerType.Clock then {
        t = new ClockTimer();
      } else if timerType == TimerType.Cycle then {
        t = new CycleTimer();
      } else {
        halt("Unknown timer type");
      }
    }
    proc deinit() {
      if t != nil then delete t;
    }

    proc start() {
      t.start();
    }

    proc stop() {
      t.stop();
      was_run = true;
    }
    proc elapsed() {
      return t.elapsed();
    }
  }

  proc copyTimer(loop_stat: LoopStat, ilength: LoopLength, loop_timer: LoopTimer) {
    if loop_timer.was_run {
      const run_time = loop_timer.elapsed();
      loop_stat.loop_run_time[ilength].push_back(run_time);
    }
  }
}
