use Time;
use CommDiagnostics;
use BlockDist;

config const numTrials = 1;
config const numIters = 500000;
config const numTasks = here.maxTaskPar;
config const printTime = true;
config const printDiags = false;
config type baseType = int(64);

enum OP {GET,PUT,FAMO,NFAMO,CASAMO,GETAMO,PUTAMO,FASTAM,AM,CFAMO};

var t: stopwatch;
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

  proc init(type T) {
    this.T = T;
  }
  proc init=(other: padded(?)) {
    this.T = other.T;
    // I think `pad` does not need to be copied here
    // it is an optimization to make the struct consume a whole cache line
    if(isAtomicType(T)) {
      this.val = other.val.read();
    }
    else {
      this.val = other.val;
    }
  }
}

// Create arrays and warmup / init RAD cache
type atomicType = atomic baseType;
var A = blockDist.createArray(1..numTasks*2, padded(atomicType));
var B = blockDist.createArray(1..numTasks*2, padded(atomicType.valType));
for loc in Locales do on loc {
  coforall tid in 1..numTasks*2 with (ref A, ref B) {
    A[tid].val.write(0);
    B[tid].val = 0;
  }
}

// Test different OPS
proc test(op: OP) {
  const iters = if op == OP.AM || op == OP.FASTAM then numIters/10 else numIters;
  startDiags();
  coforall tid in 1..numTasks with (ref A, ref B) {
    ref bLoc = B.localAccess[tid].val;
    ref bRem = B[tid+numTasks].val;
    ref aLoc = A.localAccess[tid].val;
    ref aRem = A[tid+numTasks].val;
    ref lRem = Locales[numLocales-1];
    ref aRemShared = A[1].val;
    select op {

      // RMA
      when OP.GET    do for i in 0..<iters do bLoc = bRem;
      when OP.PUT    do for i in 0..<iters do bRem = bLoc;
      // AMO
      when OP.FAMO   do for i in 0..<iters do aRem.fetchAdd(1);
      when OP.NFAMO  do for i in 0..<iters do aRem.add(1);
      when OP.CASAMO do for i in 0..<iters:aRem.valType do aRem.compareAndSwap(i, i+1);
      when OP.GETAMO do for i in 0..<iters do bLoc = aRem.read();
      when OP.PUTAMO do for i in 0..<iters do aRem.write(bLoc);
      when OP.CFAMO  do for i in 0..<iters do aRemShared.fetchAdd(1);
      // AM
      when OP.FASTAM do for i in 0..<iters do on lRem do B.localAccess[tid].val = 0;
      when OP.AM     do for i in 0..<iters do on lRem do B.localAccess[tid].val = thwartFastOn():B.localAccess[tid].val.type;
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
