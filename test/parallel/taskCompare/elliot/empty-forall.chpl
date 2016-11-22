use Time;

config const numTrials = 100;
config const printTimings = false;

proc forallTaskSpawn(trials, numTasks) {
  for 1..trials do
    forall 1..numTasks { }
}

proc main() {
  var t: Timer;

  t.start();
  forallTaskSpawn(numTrials, here.maxTaskPar);
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}
