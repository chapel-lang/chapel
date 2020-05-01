use Memory, Time, SysCTypes;

enum op_t {
  opGet,
  opPut,
  opMisalignedGet,
  opMisalignedPut
};

use op_t;

config const op = opGet;

config type elemType = int(8);

config const memFraction = 10;
config const maxMem = here.physicalMemory(unit = MemUnits.Bytes) / memFraction;

config const xferMB = maxMem / 2**20;
config var xferMem = xferMB * 2**20;

config const verboseLimiting = false;

// apply limiting due to available memory
if xferMem > maxMem {
  xferMem = maxMem;
  if verboseLimiting then
    writeln('memory footprint limiting reduces arrays to about ',
            xferMem / 2**20, ' MB each');
}

// apply limiting due to addressability
const maxAlloc = (if numBits(size_t) == 64 then 2**48 else 2**30);
if xferMem > maxAlloc {
  xferMem = maxAlloc;
  if verboseLimiting then
    writeln('addressability limiting reduces arrays to about ',
            xferMem / 2**20, ' MB each');
}

const n = xferMem / numBytes(elemType);

proc defaultNumOpsPerTimerCheck() {
  if xferMB > 1000 then return 1;
  if xferMB > 1    then return 10;
  return 1000;
}

config const runSecs = 5.0;
config const minOpsPerTimerCheck = defaultNumOpsPerTimerCheck();

config const printTimings = false;

proc main() {
  var A: [1..n] elemType;
  [i in A.domain] A(i) = i:A.eltType;

  on Locales[numLocales - 1] {
    var nopsAtCheck = minOpsPerTimerCheck;
    var nops: int;
    var t: Timer;

    var B: [1..n] elemType;
    [i in B.domain] B(i) = (n + 1 - i):B.eltType;

    t.start();

    while true {
      if nops == nopsAtCheck {
        if t.elapsed() >= runSecs then break;
        nopsAtCheck = (nops * (0.75 * runSecs / t.elapsed())):int;
        if nopsAtCheck - nops < minOpsPerTimerCheck then
          nopsAtCheck = nops + minOpsPerTimerCheck;
      }

      // do op
      if op == opGet {
        B = A;
      } else if op == opPut {
        A = B;
      } else if op == opMisalignedGet {
        B[2..n-1] = A[2..n-1];
      } else if op == opMisalignedPut {
        A[2..n-1] = B[2..n-1];
      }

      nops += 1;
    }

    t.stop();

    if printTimings {
      writeln("Time: ", t.elapsed());
      writeln("nops: ", nops);
      writeln("MB/s: ", ((nops*xferMem):real / 2**20:real) / t.elapsed());
    }
  }
}
