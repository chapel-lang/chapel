use BufferedAtomics;

config const numTasksPerLocale = here.maxTaskPar;
const numTasks = numLocales * numTasksPerLocale;

config const iters = 10000;

config const concurrentFlushing = false;

var a: atomic int;

coforall loc in Locales do on loc {
  coforall 1..numTasksPerLocale {
    for 1..iters {
      a.addBuff(1);
      if concurrentFlushing then flushAtomicBuff();
    }
  }
}
flushAtomicBuff();

assert(a.read() == numTasks * iters);
