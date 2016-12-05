use Time;

config const numTrials = 100;
config const printTimings = false;

enum TaskingMode {
  qtChplLikeT, qtChplOptT
};
use TaskingMode;

config param taskingMode = qtChplLikeT;

proc main() {
  var t: Timer;

  t.start();
  select taskingMode {
     when qtChplLikeT do qtChplLikeTaskSpawn(numTrials, here.maxTaskPar);
     when qtChplOptT  do qtChplOptTaskSpawn (numTrials, here.maxTaskPar);
  }
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}

extern proc qtChplLikeTaskSpawn(trials, numTasks);
extern proc qtChplOptTaskSpawn(trials, numTasks);
