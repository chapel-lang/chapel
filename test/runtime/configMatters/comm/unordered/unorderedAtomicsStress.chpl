use UnorderedAtomics;

config const oversubscription = 1;
config const numTasksPerLocale = here.maxTaskPar * oversubscription;
const numTasks = numLocales * numTasksPerLocale;

config const iters = 10000;
const itersSum = iters*(iters+1)/2; // sum from 1..iters

config const concurrentFencing = false;

var a: atomic int;

coforall loc in Locales do on loc {
  coforall 1..numTasksPerLocale {
    for i in 1..iters {
      a.unorderedAdd(i);
      if concurrentFencing then unorderedAtomicTaskFence();
    }
  }
}
assert(a.read() == numTasks * itersSum);

coforall loc in Locales do on loc {
  coforall 1..numTasksPerLocale {
    for i in 1..iters {
      a.unorderedSub(i);
      if concurrentFencing then unorderedAtomicTaskFence();
    }
  }
}
assert(a.read() == 0);
