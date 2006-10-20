// standard modules for loop scheduling and timing
use Schedules;
use Time;
use Types;

// user module for computing HPCC problem sizes
use HPCCProblemSize;


// the number of tables we'll be using and the element type
param numTables = 1;
type elemType = uint(64);
type indexType = uint(64);
type randType = uint(64);
// Would like this be a param:
const hiRandBit = 0x1:randType << (numBits(randType)-1);
// TODO: Make sure I use these types consistently

// configuration constants for specifying the problem size

config const n = lg(computeProblemSize(elemType, numTables, returnPow2=true))
                 : indexType,
             N_U = 2**(n+2); // numUpdates
const m = 2**n;        // problemSize
//config const n = lg(1024): indexType;

// configuration constants for controlling output options
config const printParams = true,
             printArrays = false,
             printStats = true;

// configuration constants for controlling the degree of parallelism
config const threadsPerLocale = 5;

config const errorTolerance = 1.0e-2,
             parallelVerify = true;

def main() {


  

  const numThreads = numLocales * threadsPerLocale,
        ThreadSpace: domain(1) distributed(Block) = [1..numThreads];

  const TableSpace: domain(1, indexType) distributed = [0..m);
  var T: [TableSpace] elemType;

  printConfiguration(numThreads);

  const startTime = getCurrentTime();

  [i in TableSpace] T(i) = i:elemType;

  forall thread, block in ThreadSpace, BlockSchedule([0..N_U), numThreads) {
    //    writeln("thread ", thread, " will execute iterations ", block);
    for i in RAStream(block.numIndices, block.low) {
      //      T(i & (m-1)) ^= i;
      T(i & (m-1)) ^= i:elemType;
    }
  }

  const execTime = getCurrentTime() - startTime;

  const validAnswer = verifyResults(T, numThreads, ThreadSpace);

  printResults(validAnswer, execTime);
}


def printConfiguration(numThreads) {
  if (printParams) {
    printProblemSize(elemType, numTables, m);
    writeln("Number of updates = ", N_U);
    writeln("Number of threads = ", numThreads);
    writeln();
  }
}


def verifyResults(T: [?TDom], numThreads, ThreadSpace) {
  if (printArrays) then writeln("After updates, T is: ", T, "\n");

  if (parallelVerify) {
    forall thread, block in ThreadSpace, BlockSchedule([0..N_U), numThreads) {
      for i in RAStream(block.numIndices, block.low) {
        //        atomic { T(i & (m-1)) ^= i; }
        atomic { T(i & (m-1)) ^= i:elemType; }
      }
    }
  } else {
    for i in RAStream(N_U) {
      //      T(i & (m-1)) ^= i;
      T(i & (m-1)) ^= i:elemType;
    }
  }

  const numErrors = + reduce [i in TDom] (T(i) != i:elemType):int;

  if (printArrays) then writeln("After verification, T is: ", T, "\n");

  if (printStats) then writeln("Number of errors is: ", numErrors, "\n");

  return numErrors <= (errorTolerance * N_U);
}


def printResults(successful, execTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time = ", execTime);
    const GUPS = N_U / execTime * 1.0e-9;
    writeln("Performance (GUPS) = ", GUPS);
  }
}


iterator RAStream(numvals, start:randType = 0): indexType {
  const POLY = 0x7:indexType;
  var val = getNthRandom(start): indexType;
  for i in 1..numvals {
    val = (val << 1) ^ (if (val & hiRandBit) then POLY else 0);
    yield val;
  }
}


def getNthRandom(in n: randType) {
  param period = 1317624576693539401;

  while (n & hiRandBit) do
    n += period;
  while (n > period) do
    n -= period;
  if (n == 0) then return 0x1:elemType;

  param m2DomSize = 64; // BLC: this is a magic number -- better name?
  var m2Dom = [0..m2DomSize-1];
  var m2: [m2Dom] elemType;

  // BLC: isn't this redundantly computing the same array m2 over and
  // over again?
  var temp = 0x1: elemType;  // BLC: is there a better name for this?
  for i in m2Dom {
    m2(i) = temp;
    bitMunge(temp);
    bitMunge(temp);
  }

  var high = 62;    // BLC: magic number -- name?
  while (n >> high) & 1 == 0 do
    high -= 1;

  var ran = 0x2: elemType;
  forall i in 0..high-1 by -1 {
    var temp = 0:elemType;  // BLC: is there a better name for this?
    for j in m2Dom {
      if ((ran >> j) & 1) then temp ^= m2(j);
    }
    ran = temp;
    if ((n >> i) & 1) {
      bitMunge(ran);
    }
  }
  return ran;
}


// BLC: would also like to see this fn inlined -- how to specify?
// BLC: better name for this fn?
def bitMunge(inout x) {
  param POLY = 0x7:indexType;
  //  x = (x << 1) ^ (if (x & hiIndBit) then POLY else 0);
  x = ((x:randType << 1) ^ (if (x:randType & hiRandBit) then POLY else 0)):x.type;
}

