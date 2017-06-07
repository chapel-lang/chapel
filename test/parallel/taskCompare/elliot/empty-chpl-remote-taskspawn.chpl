use Time;

config const numTrials = 100;
config const printTimings = false;

enum TaskingMode {
  coforallOnT
};
use TaskingMode;

config param taskingMode = coforallOnT;

proc main() {
  var t: Timer;

  t.start();
  select taskingMode {
     when coforallOnT do coforallOnTaskSpawn(numTrials);
  }
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}


proc coforallOnTaskSpawn(trials) {
  for 1..numTrials do
    coforall loc in Locales do on loc { }
}

