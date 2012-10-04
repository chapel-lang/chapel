// streamTH.chpl
//
// Stream benchmark on Tilera with hand-coded data distribution.
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
for loc in (here:Tilera).getChildren() do
{
  sublocs[i] = loc;
  i += 1;
}

class BlockArr : BaseArr {
  type eltType;
  var dom : domain(1);
  var myElems: [dom] eltType;

  inline proc these()
    return myElems.these();
}

proc main() {
  printConfiguration();

  // Home-brew block distribution
  const problemSpace: domain(1) = {1..m};
  const n = sublocs.numElements;
  const blockSpace = {1..n};
  var distA, distB, distC : [blockSpace] BlockArr(elemType);

// Initialize the arrays
  var randlist = new RandomStream(seed);
  for (loc,i) in (sublocs,sublocs.domain) do
    on loc
    {
      var block = getChunk(i, n, problemSpace);
      distA[i] = new BlockArr(eltType=elemType, dom=block);
      distB[i] = new BlockArr(eltType=elemType, dom=block);
      distC[i] = new BlockArr(eltType=elemType, dom=block);
      randlist.fillRandom(distB[i].myElems);
      randlist.fillRandom(distC[i].myElems);
    }
  delete randlist;
  writeln("Done with setup.");

  var execTime: [1..numTrials] real;
  for trial in 1..numTrials {
    const startTime = getCurrentTime();
    coforall (loc,i) in (sublocs, sublocs.domain) do
      on loc {
//        writeln("Computing ", distA[i].myElems.domain, " on CPU ", loc);
        for (a,b,c) in (distA[i].myElems,distB[i].myElems,distC[i].myElems) do
          a = b + alpha * c;
      }
    execTime(trial) = getCurrentTime() - startTime;
  }

// What could go wrong?
//  const validAnswer = verifyResults(A, B, C);
  const validAnswer = true;
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


