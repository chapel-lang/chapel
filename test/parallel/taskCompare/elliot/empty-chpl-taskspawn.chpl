use Time;

config const numTrials = 100;
config const printTimings = false;

enum TaskingMode {
  forBeginT, coforallT, forallT,
  serialForBeginT, serialCoforallT, serialForallT
};
use TaskingMode;

config param taskingMode = forallT;

proc main() {
  var t: Timer;

  t.start();
  select taskingMode {
     when forBeginT       do           forBeginTaskSpawn(numTrials, here.maxTaskPar);
     when coforallT       do           coforallTaskSpawn(numTrials, here.maxTaskPar);
     when forallT         do           forallTaskSpawn  (numTrials, here.maxTaskPar);
     when serialForBeginT do serial do forBeginTaskSpawn(numTrials, here.maxTaskPar);
     when serialCoforallT do serial do coforallTaskSpawn(numTrials, here.maxTaskPar);
     when serialForallT   do serial do forallTaskSpawn  (numTrials, here.maxTaskPar);
  }
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}


proc forBeginTaskSpawn(trials, numTasks) {
  for 1..numTrials do
    sync { for 1..numTasks do begin { } }
}

proc coforallTaskSpawn(trials, numTasks) {
  for 1..trials do
    coforall 1..numTasks { }
}

proc forallTaskSpawn(trials, numTasks) {
  for 1..trials do
    forall 1..numTasks { }
}

