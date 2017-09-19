use Time;

type elemType = uint;

config const numMB = 2 * 1024; // much larger than Gemini/Aries max xfer (1gb)
const n = (numMB * 1024 * 1024) / numBytes(elemType);

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

  if verify then
    writeln(if && reduce [i in 1..n by verifyStride] B(i) == A(i) then
              'PASS'
            else 
              'FAIL');

  if showPerf then
    writeln("Time: ", elapsedTime);
}
