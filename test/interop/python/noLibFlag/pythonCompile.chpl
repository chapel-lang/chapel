// Chapel file that exports a variety of functions for use with Python
export proc foo() {
  writeln("I do nothing");
}

export proc takesAndReturns(x: int): int {
  writeln("arg was: ", x, ", return is: ", (x + 1));
  return x + 1;
}

export proc parallelProg(numTasksToRun: int) {
  coforall i in 1..numTasksToRun {
    writeln("In task ", i);
  }
}

export proc threadring(passes: int, tasks: int) {
  var mailbox$: [1..tasks] sync int;
  mailbox$[1] = 0;
  proc passTokens(tid) {
    do {
      const numPasses = mailbox$[tid];
      mailbox$[tid%tasks+1] = numPasses+1;
      if numPasses == passes then
        writeln(tid);
    } while (numPasses < passes);
  }
  coforall tid in 1..tasks do
    passTokens(tid);
}