// tilera.chpl
//

use tilera36;
use BlockDist;

//################################################################################{
//# Streams copied verbatim here.
//#
use Time;
use Types;
use Random;

use HPCCProblemSize;


param numVectors = 3;
type elemType = real(64);

config const m = computeProblemSize(elemType, numVectors),
             alpha = 3.0;

config const numTrials = 10,
             epsilon = 0.0;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.currentTime else 314159265;

config const printParams = true,
             printArrays = false,
             printStats = true;

var sublocD = {1..here.getChildCount()};
var sublocs : [sublocD] locale;

// Enumerate the child locales.
var i = 1;
for loc in here.getChildren() do
{
  sublocs[i] = loc;
  i += 1;
}

proc main() {
  printConfiguration();

  const ProblemSpace: domain(1) = {1..m};
  const ProblemSpaceDist = ProblemSpace
    dmapped Block(boundingBox=ProblemSpace, targetLocales=sublocs);
  var A, B, C: [ProblemSpace] elemType;
  var execTime: [1..numTrials] real;

  initVectors(B, C);

  for trial in 1..numTrials {
    const startTime = getCurrentTime();
    [i in ProblemSpaceDist] A(i) = B(i) + alpha * C(i);
    execTime(trial) = getCurrentTime() - startTime;
  }

  const validAnswer = verifyResults(A, B, C);
  printResults(validAnswer, execTime);
}


proc getChunk(nChunk:int, chunkCount, problemSpace)
{
  // The CPU number is one-based
  if nChunk < 1 || nChunk > chunkCount then
    halt("Invalid chunk number");
  var low = problemSpace.low;
  var high = problemSpace.high;
  var size = 1 + (high - low);
  var start = ((nChunk-1) * size) / chunkCount + low;
  var end = (nChunk * size) / chunkCount + low - 1;
  return {start..end};
}

proc printConfiguration() {
  if (printParams) {
    printProblemSize(elemType, numVectors, m);
    writeln("Number of trials = ", numTrials, "\n");
  }
}


proc initVectors(B, C)
{
  if B.domain != C.domain then
    halt("vector lengths must match.");

  var randlist = new RandomStream(seed);
  randlist.fillRandom(B);
  randlist.fillRandom(C);

  if (printArrays) {
    writeln("B is: ", B, "\n");
    writeln("C is: ", C, "\n");
  }

  delete randlist;
}


proc verifyResults(A, B, C) {
  if (printArrays) then writeln("A is: ", A, "\n");

  const infNorm = max reduce [i in A.domain] abs(A(i) - (B(i) + alpha * C(i)));

  return (infNorm <= epsilon);
}


proc printResults(successful, execTimes) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    const maxTime = max reduce execTimes,
      totalTime = + reduce execTimes,
          avgTime = totalTime / numTrials,
          minTime = min reduce execTimes;
    writeln("Execution time:");
    writeln("  max ~= ", maxTime);
    writeln("  avg ~= ", avgTime);
    writeln("  min ~= ", minTime);

    const GBPerSec = numVectors * numBytes(elemType) * (m/minTime) * 1.0e-9;
    writeln("Performance (GB/s) ~= ", GBPerSec);
  }
}

//################################################################################}

writeln("Done.");

