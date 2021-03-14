/*
   This program measures OpenMP task spawn performance in a combined
   executable. It is run with QT_AFFINITY=no to avoid runtime contention
   but it might be reasonable to have a test that runs a standalone
   OpenMP C program to avoid the possibility of runtime contention.

   A Chapel program that calls OpenMP is a reasonable thing to do;
   it just might not be the best way to compare task startup times.

   However testing a standalone C program isn't currently easy to do.
   Some challenges are:
     * we don't currently have perf testing for .test.c tests
     * we don't currently have a way to launch C programs in the test infra
     * possibility for contention exists if the Chapel program is running
       at all (so e.g. Spawn wouldn't help)
     * OpenMP implementations don't tend to infer the number of threads,
       so we'd have to get something equivalent to here.maxTaskPar

   It might be possible to replace the _real program with a C program
   compiled in a .preexec (in launcher configurations) but that doesn't
   solve the last issue.
 */

use Time;

config const numTrials = 100;
config const printTimings = false;

enum TaskingMode {
    ompParallelForT, ompSerialParallelForT
};
use TaskingMode;

config param taskingMode = ompParallelForT;

proc main() {
  var t: Timer;

  t.start();
  select taskingMode {
     when ompParallelForT       do ompTaskSpawn(numTrials, here.maxTaskPar, false);
     when ompSerialParallelForT do ompTaskSpawn(numTrials, here.maxTaskPar, true);
  }
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}

extern proc ompTaskSpawn(trials, numTasks, runSerial);
