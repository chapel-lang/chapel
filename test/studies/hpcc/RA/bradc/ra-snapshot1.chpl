// standard modules for loop scheduling and timing
use Schedules;
use Time;

// user module for computing HPCC problem sizes
use HPCCProblemSize;


// the number of tables we'll be using and the element type
param numTables = 1;
type elemType = uint(64);
type indexType = int(64);
// TODO: Make sure I use these types consistently

// configuration constants for specifying the problem size

config const n = lg(computeProblemSize(elemType, numTables, returnPow2=true))
                 : indexType;
//config const n = lg(1024): indexType;

// configuration constants for controlling output options
config const printParams = true,
             printArrays = false,
             printTiming = true;

// configuration constants for controlling the degree of parallelism
config const threadsPerLocale = 5;
config param blockSize = 64;            // TODO: const?


def main() {
  const m = 2**n;        // problemSize
  const N_U = 2**(n+2);  // numUpdates

  const numThreads = numLocales * threadsPerLocale,
        ThreadSpace: domain(1) distributed(Block) = [1..numThreads];

  //  const bigStep:int = N_U/numThreads;  // TODO: rename

  const TableSpace: domain(1, indexType) distributed = [0..m);
  var T: [TableSpace] elemType;

  const RandStepsDomain = [0..64);  // TODO: remove magic number
  var RandomSteps: [RandStepsDomain] uint(64);

  //  const BigStepDomain = [0..bigStep);
  //  const LittleStepDomain = [0..blockSize);

  printConfiguration(m, N_U, numThreads, blockSize);

  //  initRandomSteps();

  const startTime = getCurrentTime();

  [i in TableSpace] T(i) = i:elemType;

  forall thread, block in ThreadSpace, BlockSchedule([0..N_U), numThreads) {
    writeln("thread ", thread, " will execute iterations ", block);
    for i in RAStream(block.numIndices, block.low) {
      var j = i & (m-1):indexType;
      writeln(j:uint(64), " ", i:uint(64));
    }

    
    //    writeln("using chunks as follows:");
    /*    for chunk in FixedBlockSchedule(block, blockSize) {
      writeln("  ", chunk);
      
      }*/

  }
  /*  
  forall j in [0..numThreads) {
    var ranvec: [LittleStepDomain] uint(64);
    [k in LittleStepDomain] ranvec(k) = randomStart(bigStep*j+(bigStep/blockSize)*k);
    for i in BigStepDomain by blockSize{
      for k in LittleStepDomain{
        ranvec(k) = (ranvec(k) << 1) ^ (if (ranvec(k):int(64) < 0) then POLY else 0);
        atomic {Table(ranvec(k) & (m-1)) ^= ranvec(k);}
      }
    }
  }

  const execTime = getCurrentTime() - startTime;

  const GUPs = (if (execTime > 0.0) then (1.0 / execTime) else -1.0);
  GUPs *= 1.0e-9*N_U;

  if (printTiming) then writeRAresults();

  verifyResults();
  */
}


def printConfiguration(m, N_U, numThreads, blockSize) {
  if (printParams) {
    printProblemSize(elemType, numTables, m);
    writeln("Number of updates = ", N_U);
    writeln("Number of threads = ", numThreads);
    writeln("Inner loop length = ", blockSize);
    writeln();
  }
}

/*
def randomStart(step0:int):uint(64) {

  var i:int;
  var ran:uint(64) = 2;

  if (step0 ==0) then 
    return 1;
  else
    i = lg(step0);
  while (i > 0) do {
    var temp:uint(64) = 0;
    [j in RandStepsDomain] if (( ran >> j) & 1) then temp ^= RandomSteps(j);
    ran = temp;
    i -= 1;
    if (( step0 >> i) & 1) then
      ran = (ran << 1) ^ (if (ran:int(64) < 0) then POLY else 0);
  }
  return ran;
}
*/

/*
def initRandomSteps() {
  
  var temp:uint(64) = 1;

  for i in RandStepsDomain {
    RandomSteps(i) = temp;
    temp = (temp << 1) ^ (if (temp:int(64) < 0) then POLY else 0);
    temp = (temp << 1) ^ (if (temp:int(64) < 0) then POLY else 0);
  }
}
*/

def verifyResults() {
  /*
  config const errorTolerance = 0.01;

  var temp: uint(64) = 1;  
  for i in 1..N_U {
    temp = (temp << 1) ^ (if (temp:int(64) < 0) then POLY else 0);
    T(temp & (m-1)) ^= temp;  
  }

  var numErrors = 0;
  for i in TableSpace {
    if (T(i) != i) then
      numErrors += 1;
  }

  write("Found ", numErrors, " errors in ", m, " locations ");
  if (numErrors <= errorTolerance*m) {
    writeln("(passed)");
  } else {
    writeln("(failed)");
  }
  */
}


def writeRAresults() {
  /*
  writeln("Real time used = ", execTime, " seconds");
  writeln(GUPs, " Billion(10^9) Updates    per second [GUP/s]");
  writeln();
  */
}


iterator RAStream(numvals, start=0): indexType {
  const POLY = 0x7:indexType;
  const hibit = 0x1:indexType << (numBits(indexType)-1);
  var val = getNthRandom(start): indexType;
  for i in 1..numvals {
    val = (val << 1) ^ (if (val & hibit) then POLY else 0);
    yield val;
  }
}


def getNthRandom(in n:int(64)) {
  param period = 1317624576693539401;

  while (n < 0) do
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
  param hibit = 0x1:indexType << 63;
  //  x = (x << 1) ^ (if (x & hibit) then POLY else 0);
  x = ((x:indexType << 1) ^ (if (x:indexType & hibit) then POLY else 0)):x.type;
}

