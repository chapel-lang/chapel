use MemDiagnostics, Time, CTypes, CommDiagnostics;

type elemType = int;

config const memFraction = 3;
config const maxMem = here.physicalMemory(unit = MemUnits.Bytes) / memFraction;

// default xferMem (2gb) is much larger than Aries xfer max (1gb)
config const xferMB = 2**11;
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
const maxAlloc = (if numBits(c_size_t) == 64 then 2**48 else 2**30);
if xferMem > maxAlloc {
  xferMem = maxAlloc;
  if verboseLimiting then
    writeln('addressability limiting reduces arrays to about ',
            xferMem / 2**20, ' MB each');
}

const n = xferMem / numBytes(elemType);

config const doGET = true;

// If small enough, a non-unit verifyStride can reduce communication
// while maintaining verification quality.  The default checks every
// 4kb system page and lets us verify a 2gb array assignment on Cray X*
// in <10secs.
config const verify = true;
config const verifyStride = (2**12) / numBytes(elemType);
config const printDiags = false;
config const showPerf = false;


var t: stopwatch;
proc startDiags() {
  if printDiags { startCommDiagnostics(); }
  t.start();
}
proc stopDiags(size) {
  t.stop();
  if showPerf {
    writeln("Size: ", size);
    var xferGB = size / (2**30);
    writef("GB: %.2dr\n", xferGB);
    writeln("Time: ", t.elapsed());
    writef("GB/s: %.2dr\n", xferGB / t.elapsed());
  }
  t.clear();
  if printDiags {
    stopCommDiagnostics();
    printCommDiagnosticsTable();
    resetCommDiagnostics();
  }
}

var A: [1..n] elemType;
[i in A.domain with (ref A)] A(i) = i:A.eltType;


on Locales[numLocales - 1] {
  var B: [1..n] elemType;
  [i in B.domain with (ref B)] B(i) = (n + 1 - i):B.eltType;

  startDiags();
  if doGET then
    B = A;
  else
    A = B;
  stopDiags(xferMem);

  if verify {
    const arraysMatch = && reduce [i in 1..n by verifyStride] B(i) == A(i);
    writeln(if arraysMatch then 'PASS' else 'FAIL');
  }
}
