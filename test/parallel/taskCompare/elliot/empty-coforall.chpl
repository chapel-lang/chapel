use Time;

config const numTrials = 100;
config const printTimings = false;

proc main() {
  var t: Timer;

  t.start();
  for 1..numTrials {
    coforall 1..here.maxTaskPar {
    }
  }
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}
