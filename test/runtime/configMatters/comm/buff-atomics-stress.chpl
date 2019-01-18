use BufferedAtomics;

config const numTasksPerLocale = here.maxTaskPar;
const numTasks = numLocales * numTasksPerLocale;

config const iters = 10000;
const itersSum = iters*(iters+1)/2; // sum from 1..iters

config const concurrentFlushing = false;

var a: atomic int;

coforall loc in Locales do on loc {
  coforall 1..numTasksPerLocale {
    for i in 1..iters {
      a.addBuff(i);
      if concurrentFlushing then flushAtomicBuff();
    }
  }
}
flushAtomicBuff();
assert(a.read() == numTasks * itersSum);

coforall loc in Locales do on loc {
  coforall 1..numTasksPerLocale {
    for i in 1..iters {
      a.subBuff(i);
      if concurrentFlushing then flushAtomicBuff();
    }
  }
}
flushAtomicBuff();
assert(a.read() == 0);
