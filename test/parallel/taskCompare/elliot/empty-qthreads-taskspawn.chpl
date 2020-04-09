use Time;

config const numTrials = 100;
config const printTimings = false;

enum TaskingMode {
  qtChplLikeT, qtChplOptT,
  qtSerialChplLikeT, qtSerialChplOptT
};
use TaskingMode;

config param taskingMode = qtChplLikeT;

proc main() {
  var t: Timer;

  t.start();
  select taskingMode {
     when qtChplLikeT       do qtChplLikeTaskSpawn(numTrials, here.maxTaskPar, false);
     when qtChplOptT        do qtChplOptTaskSpawn (numTrials, here.maxTaskPar, false);
     when qtSerialChplLikeT do qtChplLikeTaskSpawn(numTrials, here.maxTaskPar, true);
     when qtSerialChplOptT  do qtChplOptTaskSpawn (numTrials, here.maxTaskPar, true);
  }
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}

extern proc qtChplLikeTaskSpawn(trials, numTasks, runSerial);
extern proc qtChplOptTaskSpawn(trials, numTasks, runSerial);
