use Memory, Time, SysCTypes;

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
const maxAlloc = (if numBits(size_t) == 64 then 2**48 else 2**30);
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

config const showPerf = false;

var A: [1..n] elemType;
[i in A.domain] A(i) = i:A.eltType;

on Locales[numLocales - 1] {
  var B: [1..n] elemType;
  [i in B.domain] B(i) = (n + 1 - i):B.eltType;

  const startTime = getCurrentTime();
  if doGET then
    B = A;
  else
    A = B;
  const elapsedTime = getCurrentTime() - startTime;

  if verify {
    const arraysMatch = && reduce [i in 1..n by verifyStride] B(i) == A(i);
    writeln(if arraysMatch then 'PASS' else 'FAIL');
  }

  if showPerf then
    writeln("Time: ", elapsedTime);
}
