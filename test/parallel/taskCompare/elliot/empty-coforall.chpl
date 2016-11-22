use Time;

config const numTrials = 100;
config const printTimings = false;

proc coforallTaskSpawn(trials, numTasks) {
  for 1..trials do
    coforall 1..numTasks { }
}

proc main() {
  var t: Timer;

  t.start();
  coforallTaskSpawn(numTrials, here.maxTaskPar);
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}
