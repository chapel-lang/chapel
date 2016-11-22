use Time;

config const numTrials = 100;
config const printTimings = false;

proc forBeginTaskSpawn(trials, numTasks) {
  for 1..numTrials do
    sync { for 1..numTasks do begin { } }
}

proc main() {
  var t: Timer;

  t.start();
  forBeginTaskSpawn(numTrials, here.maxTaskPar);
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}
