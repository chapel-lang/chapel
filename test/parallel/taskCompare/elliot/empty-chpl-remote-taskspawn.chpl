use Time;

config const numTrials = 100;
config const printTimings = false;
// bogus config const to prevent compiler eliminating tup copy
config const printTup = false;

enum TaskingMode {
  emptyCoforallOn,
  medCoforallOn,
  largeCoforallOn
};
use TaskingMode;

config param taskingMode = emptyCoforallOn;

proc main() {
  var t: Timer;

  t.start();
  select taskingMode {
     when emptyCoforallOn do coforallOnTaskSpawn();
     when medCoforallOn   do coforallOnTaskSpawnMed();
     when largeCoforallOn do coforallOnTaskSpawnLarge();
  }
  t.stop();
  verifyCounter();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}


// Empty task spawn
proc coforallOnTaskSpawn() {
  for 1..numTrials do
    coforall loc in Locales do on loc { incCounter(); }
}

// 512 byte payload task spawn. At least in 2018 this represents the typical
// size of remote forks for our multi-locale performance suite
proc coforallOnTaskSpawnMed() {
  var tup: 64*int;
  for 1..numTrials do
    coforall loc in Locales with (in tup) do on loc { 
      if printTup then writeln(tup);
      incCounter();
    }
}

// 4096 byte payload task spawn.
proc coforallOnTaskSpawnLarge() {
  var tup: 512*int;
  for 1..numTrials do
    coforall loc in Locales with (in tup) do on loc { 
      if printTup then writeln(tup);
      incCounter();
    }
}

pragma "locale private" var counter = 0;
inline proc incCounter() { counter += 1; }
inline proc getCounter() { return counter; }
inline proc verifyCounter() {
  coforall loc in Locales do on loc do
    assert(getCounter() == numTrials);
}
