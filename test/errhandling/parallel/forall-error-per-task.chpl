// similar to test/errhandling/parallel/forall-calls-throwing-fn2.chpl
// but reliably reproduces a race condition that resulted
// in hangs due to a cycle in a TaskErrors/chpl_TaskErrors object.
use Time, BlockDist;

config const nTasks = 4;
config const delay = 0.1;
config const which = 1;

record sleeper {
  proc deinit() { sleep(delay); }
}

proc throwingFunction() throws {
  var s: sleeper;
  throw new Error("die!");
}

iter myIter() {
  for i in 1..nTasks do yield i;
}

iter myIter(param tag: iterKind) where tag == iterKind.leader {
  coforall loc in Locales do on loc {
    for f in (1..nTasks).these(tag) do yield f;
  }
}

iter myIter(param tag: iterKind, followThis) where tag == iterKind.follower {
  for i in followThis(0) do yield i + 1;
}

proc throwingCaller() throws {
  if which == 1 then
    forall i in myIter() do throwingFunction();
  else {
    var A = blockDist.createArray(1..nTasks, int);
    forall a in A do throwingFunction();
  }
}

proc main() {
  try { throwingCaller(); }
  catch { writeln("Caught an error!"); }
}
