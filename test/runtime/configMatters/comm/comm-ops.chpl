use Time;
use CommDiagnostics;
use BlockDist;

config const numTrials = 1;
config const numIters = 500000;
config const numTasks = here.maxTaskPar;
config const printTime = true;
config const printDiags = false;

enum OP {GET,PUT,FAMO,NFAMO,CASAMO,GETAMO,PUTAMO,FASTAM,AM};

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

inline proc thwartFastOn() {
  extern proc sizeof(type x): int;
  return sizeof(int);
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
for loc in Locales do on loc {
  coforall tid in 1..numTasks*2 {
    A[tid].val.write(0);
    B[tid].val = 0;
  }
}

// Test different OPS
proc test(op: OP) {
  const iters = if op == OP.AM || op == OP.FASTAM then numIters/10 else numIters;
  startDiags();
  coforall tid in 1..numTasks {
    ref bLoc = B.localAccess[tid].val;
    ref bRem = B[tid+numTasks].val;
    ref aLoc = A.localAccess[tid].val;
    ref aRem = A[tid+numTasks].val;
    ref lRem = Locales[numLocales-1];
    select op {

      // RMA
      when OP.GET    do for i in 0..<iters do bLoc = bRem;
      when OP.PUT    do for i in 0..<iters do bRem = bLoc;
      // AMO
      when OP.FAMO   do for i in 0..<iters do aRem.fetchAdd(1);
      when OP.NFAMO  do for i in 0..<iters do aRem.add(1);
      when OP.CASAMO do for i in 0..<iters do aRem.compareAndSwap(i, i+1);
      when OP.GETAMO do for i in 0..<iters do bLoc = aRem.read();
      when OP.PUTAMO do for i in 0..<iters do aRem.write(bLoc);
      // AM
      when OP.FASTAM do for i in 0..<iters do on lRem do B.localAccess[tid].val = 0;
      when OP.AM     do for i in 0..<iters do on lRem do B.localAccess[tid].val = thwartFastOn();
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
