use AllLocalesBarriers;

// Barrier across all locales
coforall loc in Locales do on loc {
  writeln("Before barrier");
  allLocalesBarrier.barrier();
  writeln("After barrier");
}

writeln();

// Barrier across tasks locally before doing a barrier across locales
const numTasksPerLocale = 4;
allLocalesBarrier.reset(numTasksPerLocale);

coforall loc in Locales do on loc {
  coforall tid in 1..numTasksPerLocale {
    writeln("Before barrier");
    allLocalesBarrier.barrier();
    writeln("After barrier");
  }
}
