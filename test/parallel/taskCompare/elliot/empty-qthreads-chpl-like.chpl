use Time;

config const numTrials = 100;
config const printTimings = false;

extern proc qtChplLikeTaskSpawn(trials, numTasks);

proc main() {
  var t: Timer;

  t.start();
  qtChplLikeTaskSpawn(numTrials, here.maxTaskPar);
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}
