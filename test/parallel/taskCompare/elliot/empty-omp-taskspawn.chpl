use Time;

config const numTrials = 100;
config const printTimings = false;

extern proc ompTaskSpawn(trials, numTasks);

proc main() {
  var t: Timer;

  t.start();
  ompTaskSpawn(numTrials, here.maxTaskPar);
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}
