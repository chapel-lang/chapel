use Memory, Time;

type elemType = int;

config const maxMemFraction = 1.0/3.0;
const maxMem = (here.physicalMemory(unit = MemUnits.Bytes) * maxMemFraction)
               :int(64);
const maxElems = maxMem / numBytes(elemType);

config const verboseLimiting = false;

config const numMB = 2 * 1024; // much larger than Gemini/Aries max xfer (1gb)
config const xferMem = numMB * 1024 * 1024;
var n = xferMem / numBytes(elemType);

if n > maxElems {
  n = maxElems;
  if verboseLimiting then
    writeln('memory footprint limiting reduces arrays to about ',
            n * numBytes(elemType) / (2**20), ' MB each');
}

config const doGET = true;

// If small enough, a non-unit verifyStride can reduce communication
// while maintaining verification quality.  The default checks every
// 4kb system page and lets us verify a 2gb array assignment on Cray X*
// in <10secs.
config const verify = true;
config const verifyStride = (4 * 1024) / numBytes(elemType);

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
