use Time;

config const remoteTasksCreatedPerTask = if CHPL_COMM == 'ugni' then 100000
                                                                else 2000;
config const numTasksCreating = here.maxTaskPar;
config const printTimings = false;

// From the last locale, have each task create a bunch of tasks back on locale
// 0. We should be able to create an arbitrary number of "concurrent" tasks
// under qthreads so long as there's no task yields in the user-code.
var counter: chpl__processorAtomicType(int);
var t: Timer; t.start();
on Locales[numLocales-1] {
  coforall 1..numTasksCreating {
    coforall 1..remoteTasksCreatedPerTask do on Locales[0] {
      counter.add(1);
    }
  }
}
t.stop();
assert(counter.read() == numTasksCreating * remoteTasksCreatedPerTask);

if printTimings {
  writeln(t.elapsed());
}
