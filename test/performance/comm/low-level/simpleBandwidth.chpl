use Time;
use CommDiagnostics;
use BlockDist;
use Communication;
use CTypes;

config const numTasks = here.maxTaskPar;
config const numTrials = 1;
config const printTime = true;
config const printDiags = false;
config const sizePerTask = 8192 * 16;

enum OP {GET, PUT}

var t: stopwatch;
proc startDiags() {
  t.start();
  if printDiags { startCommDiagnostics(); }
}
proc stopDiags(op: OP) {
  t.stop();
  if printTime {
    writef("%6s %2.2drs (%.2dr MiB/s)\n", op:string, t.elapsed(), (numTasks*sizePerTask):real/(2**20):real/t.elapsed());
  }
  t.clear();
  if printDiags {
    stopCommDiagnostics();
    printCommDiagnosticsTable();
    resetCommDiagnostics();
  }
}

var A = blockDist.createArray(0..<numTasks*2*sizePerTask, uint(8));

// Test different OPS
proc test(op: OP, warmup=false) {
  if !warmup then startDiags();
  coforall tid in 0..<numTasks with (ref A) {
    ref aLoc = A.localAccess[tid*sizePerTask];
    ref aRem = A[(numTasks*sizePerTask) + tid*sizePerTask];
    select op {
      when OP.GET do get(c_ptrTo(aLoc), c_ptrTo(aRem), 1, sizePerTask);
      when OP.PUT do put(c_ptrTo(aRem), c_ptrTo(aLoc), 1, sizePerTask);
    }
  }
  if !warmup then stopDiags(op);
}

if printTime || printDiags then
  writeln("Running with ", numTasks, " tasks\n");

test(OP.GET, warmup=true);
test(OP.PUT, warmup=true);
for 1..numTrials {
  test(OP.GET);
  test(OP.PUT);
}
