use Time;
use Barriers;

config const numTrials = 100;
config const printTimings = false;

config const numTasksPerLocale = here.maxTaskPar;
const numTasks = numLocales * numTasksPerLocale;

enum BarrierMode {
  LocalAtomic
};
use BarrierMode;

config param barrierMode = LocalAtomic;

proc main() {
  var t: Timer;

  var LocalAtomicBarrier = new Barrier(numTasks, BarrierType.Sync);

  t.start();
  select barrierMode {
    when LocalAtomic do LocalBarrierBarrier();
  }
  t.stop();

  if printTimings {
    writeln("Elapsed time: ", t.elapsed());
  }
}

proc LocalBarrierBarrier() {
  var barrier = new Barrier(numTasks, BarrierType.Atomic);
  coforall loc in Locales do on loc do
    coforall 1..numTasksPerLocale do
      for 1..numTrials do
        barrier.barrier();
}
