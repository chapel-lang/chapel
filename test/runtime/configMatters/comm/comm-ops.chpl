use Time;
use CommDiagnostics;
use BlockDist;

config const numTrials = 1;
config const numIters = 500000;
config const numTasks = here.maxTaskPar;
config const printTime = true;
config const printDiags = false;

enum OP {GET,PUT,FAMO,NFAMO,CASAMO,GETAMO,PUTAMO,AM};

var t: Timer;
proc startDiags() {
  t.start();
  if printDiags { startCommDiagnostics(); }
}
proc stopDiags(op: OP, iters) {
  t.stop();
  if printTime {
    writef("%6s %2.2drs (%.2dr Mops/s)\n", op:string, t.elapsed(), (iters*numTasks)/1e6/t.elapsed());
  }
  t.clear();
  if printDiags {
    stopCommDiagnostics();
    printCommDiagnosticsTable();
    resetCommDiagnostics();
  }
}

// Pad to avoid any cacheline contention
record padded {
  type T;
  var val: T;
  var pad: 7*T;
}

// Create arrays and warmup / init RAD cache
var A = newBlockArr(1..numTasks*2, padded(atomic int));
var B = newBlockArr(1..numTasks*2, padded(int));
on Locales[numLocales-1] {
  coforall tid in 1..numTasks*2 {
    A[tid].val.write(0);
    B[tid].val = 0;
  }
}

// Test different OPS
proc test(op: OP) {
  startDiags();
  const iters = if op == OP.AM then numIters/10 else numIters;
  on Locales[numLocales-1] {
    coforall tid in 1..numTasks {
      select op {
        // RMA
        when OP.GET    do for i in 0..<iters do B[tid+numTasks].val = B[tid].val;
        when OP.PUT    do for i in 0..<iters do B[tid].val = B[tid+numTasks].val;
        // AMO
        when OP.FAMO   do for i in 0..<iters do A[tid].val.fetchAdd(1);
        when OP.NFAMO  do for i in 0..<iters do A[tid].val.add(1);
        when OP.CASAMO do for i in 0..<iters do A[tid].val.compareAndSwap(i, i+1);
        when OP.GETAMO do for i in 0..<iters do B[tid+numTasks].val = A[tid].val.read();
        when OP.PUTAMO do for i in 0..<iters do A[tid].val.write(B[tid+numTasks].val);
        // AM
        when OP.AM     do for i in 0..<iters do on Locales[0] do B[tid].val = 0;
      }
    }
  }
  stopDiags(op, iters);
}

config const ops = "GET,PUT,FAMO,NFAMO";
if printTime || printDiags then
  writeln("Running with ", numTasks, " tasks\n");
for 1..numTrials {
  if ops.strip().toUpper() == "ALL" {
    for op in OP do
      test(op);
  } else {
    for opStr in ops.split(',') do
      test(opStr.strip().toUpper():OP);
  }
}
