/*
This tests measures performance of reductions over DR or Block arrays
There are four individual measurements, code-named: 00, 11, 22, 33.
See "test names" for their description.
See runTest() calls for what the number of reps and
the arrays' domains being measured.
*/

use BlockDist, Time, Memory;

config param useBlockDist = CHPL_COMM != "none";
config const perf = false; // performance or --fast mode
config const reportTime = perf;
config const reportPerfSizes = perf;

////// test names //////

const name00 = "single reduction 1d";
const name11 = "multiple reductions 1d";
const name22 = "multiple reductions 2d";
const name33 = "multiple reductions 3d";

////// sizes //////

config type elemType = int;

// large array for a single reduction
config const n00perLocale =
  if perf then here.physicalMemory() / 4 / numBytes(elemType) else 1000000;
config const n00 = numLocales * n00perLocale;

// small arrays for multiple reductions
config const elemsPerCore = 1024; // or 65536
config const numCores = if perf then here.maxTaskPar else 4;

proc myRound(arg) return (arg+0.5):int;

config const n11 = numLocales * numCores * elemsPerCore;
config const n22 = myRound(sqrt(n11));
config const n33 = myRound(cbrt(n11));

// number of reps for 11,22,33
config const M = if perf then 10000 else 10;

////// time and error reporting //////

var testName: string;
var timer: Timer;
timer.start();
var numFailures = 0;

proc start(testNameArg: string) {
  testName = testNameArg;
  timer.clear(); // keep it running
}

proc end(numErrors: int) {
  const elapsed = if reportTime then timer.elapsed():string + " sec" else "";
  const message = if numErrors then numErrors:string + " FAILURES: " else "success  ";
  writeln(message, testName, ": ", elapsed);
  if numErrors then numFailures += 1;
}

////// main //////

proc main() {

  printConfig();

  runTest(name00, 1, {1..n00});
  runTest(name11, M, {1..n11});
  runTest(name22, M, {1..n22, 1..n22});
  runTest(name33, M, {1..n33, 1..n33, 1..n33});

  if numFailures then writeln(numFailures, " FAILURES");
  else writeln("all tests succeeded");
}

proc printConfig() {
  writeln(numLocales, " locale(s)",
          "   dist: ", if useBlockDist then "Block" else "DR",
          "  sizes: ", n00, " ", n11, " ", n22**2, " ", n33**3,
          "  reps: ", M,
          "");
  if reportPerfSizes {
    writeln("n00 ", n00);
    writeln("n11 ", n11);
    writeln("n22 ", n22);
    writeln("n33 ", n33);
    writeln("M   ", M);
  }
}

proc initData(testArray) {
  testArray = 2;
  return 2 * testArray.size;
}

////// single measurement //////

proc runTest(testName, testReps, testDRdom) {
  const DOM = if useBlockDist then testDRdom dmapped Block(testDRdom)
                              else testDRdom;
  var ARR: [DOM] elemType;
  const expected = initData(ARR);
  var numErrors = 0;

  // the timed section
  start(testName);
  for testIdx in 1..testReps {
    const actual = + reduce ARR;
    if actual != expected then numErrors += 1;
  }
  end(numErrors);
}
