use Time;

config const numTrials = 100;
config const printTimings = false;

config const baseNumTasks = here.maxTaskPar;


proc main() {
  taskYield(1);
  taskYield(4);
  taskYield(16);
}

proc taskYield(oversub) {
  var t: Timer;

  t.start();
  var total: atomic int;
  coforall 1..baseNumTasks*oversub {
    var i: int;
    for 1..numTrials {
      i += 1;
      chpl_task_yield();
    }
    total.add(i);
  }
  assert(total.read() == numTrials * baseNumTasks*oversub);
  t.stop();

  if printTimings {
    writeln("Elapsed time ", oversub, ": ", t.elapsed());
  }
}
