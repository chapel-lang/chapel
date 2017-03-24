use Time;

config const numTrials = 100;
config const printTimings = false;

enum TaskingMode {
    ompParallelForT, ompSerialParallelForT
};
use TaskingMode;

config param taskingMode = ompParallelForT;

proc main() {
  var t: Timer;

  t.start();
  select taskingMode {
     when ompParallelForT       do ompTaskSpawn(numTrials, here.maxTaskPar, false);
     when ompSerialParallelForT do ompTaskSpawn(numTrials, here.maxTaskPar, true);
  }
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}

extern proc ompTaskSpawn(trials, numTasks, runSerial);
