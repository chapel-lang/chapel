module Timer {
  use LCALSDataTypes;
  use Time;

  class LoopTimer {
    var t: Timer;
    var was_run: bool;

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

  proc copyTimer(loop_stat: LoopStat, ilength: int, loop_timer: LoopTimer) {
    if loop_timer.was_run {
      const run_time = loop_timer.elapsed();
      loop_stat.loop_run_time[ilength].push_back(run_time);
    }
  }
}
