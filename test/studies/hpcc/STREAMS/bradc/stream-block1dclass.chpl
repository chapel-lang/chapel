use Time, Types, Random;
use hpccMultilocale;

use HPCCProblemSize;


param numVectors = 3;
type elemType = real(64),
     indexType = int(64);

config const m = computeProblemSize(elemType, numVectors),
             alpha = 3.0;

config const numTrials = 10,
             epsilon = 0.0;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.clockMS else 314159265;

config const printParams = true,
             printArrays = false,
             printStats = true;


// TODO: Need to put in some on clauses throughout this code


class Block1DDom {
  // parameterize this by indexType and/or locIndexType
  const whole: domain(1, indexType);
  //  SHOULD BE: const locDom: [i in LocalesDom] domain(1, indexType) = new LocBlockDom(whole);
  // TODO: Move LocalesDomain into a member variable (throughout code)
  var locDom: [LocalesDomain] LocBlock1DDom;

  def initialize() {
    [i in LocalesDomain] locDom(i) = new LocBlock1DDom(i, whole);
  }

  def these() {
    for blk in locDom do
      for ind in blk do
        yield ind;
  }

  def ind2loc(ind: indexType): index(LocalesDomain) {
    return ((ind-low)/numIndices): index(LocalesDomain);
  }


  def newArray(type elemType) {
    return new Block1DArr(elemType, this);
  }

  def dom {
    return whole;
  }

  def numIndices {
    return whole.numIndices;
  }

  def low {
    return whole.low;
  }

  def high {
    return whole.high;
  }
}


class LocBlock1DDom {
  // parameterize this by indexType and/or locIndexType
  const localeID: index(LocalesDomain);
  var myBlock: domain(1, indexType);

  def LocBlock1DDom(_localeID, Whole: domain(1, indexType)) {
    localeID = _localeID;  // is there a cooler way to do this?
    myBlock = BlockPartition(Whole, localeID, numLocales);
  }

  def these() {
    for ind in myBlock do
      yield ind;
  }

  def numIndices {
    return myBlock.numIndices;
  }

  def low {
    return myBlock.low;
  }

  def high {
    return myBlock.high;
  }
}


class Block1DArr {
  type elemType;
  var dom: Block1DDom;
  var locArr: [LocalesDomain] LocBlock1DArr(elemType);

  def initialize() {
    [i in LocalesDomain] locArr(i) = new LocBlock1DArr(elemType, dom.locDom(i));
  }

  def this(i: indexType) var {
    return locArr(dom.ind2loc(i))(i);
  }

  def these() var {
    for loc in LocalesDomain {
      for elem in locArr(loc) {
        yield elem;
      }
    }
  }

  def writeThis(x: Writer) {
    var first = true;
    for loc in LocalesDomain {
      if (locArr(loc).numElements >= 1) {
        if (first) {
          first = false;
        } else {
          x.write(" ");
        }
        x.write(locArr(loc));
      }
    }
  }

  def numElements {
    return dom.numIndices;
  }
}


class LocBlock1DArr {
  type elemType;
  const locDom: LocBlock1DDom;
  var myElems: [locDom.myBlock] elemType;

  def this(i: indexType) var {
    return myElems(i);
  }

  def these() var {
    for elem in myElems {
      yield elem;
    }
  }

  def writeThis(x: Writer) {
    x.write(myElems);
  }

  def numElements {
    return myElems.numElements;
  }

}


def main() {
  printConfiguration();

  const ProblemSpace = new Block1DDom([1..m]);

  var A = ProblemSpace.newArray(elemType), 
      B = ProblemSpace.newArray(elemType),
      C = ProblemSpace.newArray(elemType);

  initVectors(B, C);

  var execTime: [1..numTrials] real;

  for trial in 1..numTrials {
    const startTime = getCurrentTime();
    // TODO: Want    A = B + alpha * C;

    /* TODO: OR EVEN:
    forall (a, b, c) in (A, B, C) {
      a = b + alpha * c;
    }
    */
    forall (i, b, c) in (ProblemSpace, B, C) {
      A(i) = b + alpha * c;
    }
    execTime(trial) = getCurrentTime() - startTime;
  }  

  const validAnswer = verifyResults(A, B, C);
  printResults(validAnswer, execTime);
}


def printConfiguration() {
  if (printParams) {
    printProblemSize(elemType, numVectors, m);
    writeln("Number of trials = ", numTrials, "\n");
  }
}


def initVectors(B, C) {
  var randlist = new RandomStream(seed);

  // TODO: should write a fillRandom() implementation that does this
  coforall loc in LocalesDomain {
    // TODO: Need to clean this up to use more normal method names
    randlist.skipToNth(B.locArr(loc).locDom.low);
    randlist.fillRandom(B.locArr(loc).myElems);
    randlist.skipToNth(B.numElements + C.locArr(loc).locDom.low);
    randlist.fillRandom(C.locArr(loc).myElems);
  }

  if (printArrays) {
    writeln("B is: ", B, "\n");
    writeln("C is: ", C, "\n");
  }
}


def verifyResults(A, B, C) {
  if (printArrays) then writelnFragArray("A is: ", A, "\n");

  const infNorm = max reduce [i in A.dom] abs(A(i) - (B(i) + alpha * C(i)));

  return (infNorm <= epsilon);
}


def printResults(successful, execTimes) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    const totalTime = + reduce execTimes,
          avgTime = totalTime / numTrials,
          minTime = min reduce execTimes;
    writeln("Execution time:");
    writeln("  tot = ", totalTime);
    writeln("  avg = ", avgTime);
    writeln("  min = ", minTime);

    const GBPerSec = numVectors * numBytes(elemType) * (m / minTime) * 1e-9;
    writeln("Performance (GB/s) = ", GBPerSec);
  }
}
