use Time;

config const numSpawns = 1000;
config const printTimings = false;
// bogus config const to prevent compiler eliminating argBundle copy
config const printArgBundle = false;

/* Stress test a variety of remote-task spawning idioms for several arguement
 * bundle sizes and with varying levels of duplication for where we spawn to:
  - serial one-to-all (most common coforall+on idiom)
  - parallel one-to-all (all tasks on locale 0 spawn to all nodes)
  - "serial" all-to-all (not really serial, but only 1 task per node)
  - parallel all-to-all (all tasks on all nodes spawn to all nodes)
  */
proc main() {
  var t: Timer;

  t.start();
  runTaskSpawn(argBundleSize=0);
  runTaskSpawn(argBundleSize=512);
  runTaskSpawn(argBundleSize=4096);
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}

proc runTaskSpawn(param argBundleSize=0) {
  for numDupes in (1, 2, 4, 8) {
    verifyCounter(serialOneToAllSpawn(argBundleSize, numDupes));
    verifyCounter(parallelOneToAllSpawn(argBundleSize, numDupes));
    verifyCounter(serialAllToAllSpawn(argBundleSize, numDupes));
    verifyCounter(parallelAllToAllSpawn(argBundleSize, numDupes));
  }
}

// Test core `coforall loc in Locales do on loc` (coforall+on) idiom
proc serialOneToAllSpawn(param argBundleSize=0, numDupes=1) {
  const numDupes = numLocales;
  const trials = divceil(numSpawns, numDupes);
  coforallOnSpawn(argBundleSize, numDupes, trials);
  return trials * numDupes;
}

// Test coforall+on, but have all tasks on Locales 0 spawning
proc parallelOneToAllSpawn(param argBundleSize=0, numDupes=1) {
  const taskPar = here.maxTaskPar;
  const trials = divceil(divceil(numSpawns, taskPar), numDupes);
  coforall 1..taskPar {
    coforallOnSpawn(argBundleSize, numDupes, trials);
  }
  return trials * numDupes * taskPar ;
}

// Test coforall+on, but have each locale spawning
proc serialAllToAllSpawn(param argBundleSize=0, numDupes=1) {
  const trials = divceil(divceil(numSpawns, numLocales), numDupes);
  coforall loc in Locales do on loc {
    coforallOnSpawn(argBundleSize, numDupes, trials);
  }
  return trials * numDupes * numLocales;
}

// Test coforall+on, but have all tasks on all locales spawning
proc parallelAllToAllSpawn(param argBundleSize=0, numDupes=1) {
  const taskPar = here.maxTaskPar;
  const trials = divceil(divceil(divceil(numSpawns, taskPar), numLocales), numDupes);
  coforall loc in Locales do on loc {
    coforall 1..taskPar {
      coforallOnSpawn(argBundleSize, numDupes, trials);
    }
  }
  return trials * numLocales * numDupes * taskPar;
}

// Core coforall+on spawn
inline proc coforallOnSpawn(param argBundleSize, numDupes, trials) {
  param tupSize = divceil(argBundleSize, numBytes(int));
  var argBundle: if argBundleSize > 0 then tupSize*int else nothing;
  for 1..trials {
    coforall loc in duplicateLocales(numDupes) with (in argBundle) do on loc {
      if argBundleSize > 0 && printArgBundle then writeln(argBundle);
      incCounter();
    }
  }
}

// helper to duplicate locales. For `Locales=[LOCALE0, LOCALE1]` with
// numDupelicates=2 it will yield `[LOCALE0, LOCALE0, LOCALE1, LOCALE1]
iter duplicateLocales(numDuplicates) {
  for loc in Locales do
    for 1..numDuplicates do
      yield loc;
}

pragma "locale private"
var counter: chpl__processorAtomicType(int);
inline proc incCounter() { counter.add(1); }
inline proc getCounter() { return counter.read(); }
inline proc resetCounter() { counter.write(0); }
inline proc verifyCounter(expected) {
  coforall loc in Locales do on loc {
    assert(getCounter() == expected);
    resetCounter();
  }
}
