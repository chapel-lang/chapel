use Random;
use Time;

enum classVals {S, W, A, B, C};

config param probClass = classVals.S;
config const printTime = false;
config const nThreads = 4;

param seed = 314159265;

const Class = {classVals.S..classVals.C};

const TotalKeys:  [Class] int = [ 2**16, 2**20, 2**23, 2**25, 2**27 ],
      MaxKey:     [Class] int = [ 2**11, 2**16, 2**19, 2**21, 2**23 ],
      TotalIters: [Class] int = [ 10, 10, 10, 10, 10 ];

const totalKeys = TotalKeys(probClass);
const maxKey = MaxKey(probClass);
const maxIterations = TotalIters(probClass);

const D: domain(1) = {0..#totalKeys};
const E: domain(2) = {0..maxKey-1, 0..nThreads-1};

// The following should be replaced by two enumerated domain arrays of arrays
const S_testIndexArray: [0..4] int = ( 48427,17148,23627,62548,4431 );
const S_testRankArray: [0..4]  int = ( 0,18,346,64917,65463 );
const W_testIndexArray: [0..4] int = ( 357773,934767,875723,898999,404505 );
const W_testRankArray: [0..4]  int = ( 1249,11698,1039987,1043896,1048018 );
const A_testIndexArray: [0..4] int = ( 2112377,662041,5336171,3642833,4250760 );
const A_testRankArray: [0..4]  int = ( 104,17523,123928,8288932,8388264 );
const B_testIndexArray: [0..4] int = ( 41869,812306,5102857,18232239,26860214 );
const B_testRankArray: [0..4]  int = ( 33422937,10244,59149,33135281,99 );
const C_testIndexArray: [0..4] int = ( 44172927,72999161,74326391,129606274,21736814 );
const C_testRankArray: [0..4]  int = ( 61147,882988,266290,133997595,133525895 );

var testIndexArray: [0..4] int;
var testRankArray: [0..4] int;
var partialVerifyVals: [0..4] int;

var keyArray, buffer: [D] int;
var keyBuff1: [E] int;
 
var passedVerifications = 0;

proc main() {
  var time = new Timer();
  var randomStream = new owned NPBRandomStream(real, seed);
  var tempreals: [1..4] real;
  var max = maxKey / 4;

  setupPartialVerify();

  writeln(" NAS Parallel Benchmarks (Chapel) - IS Benchmark\n");
  writeln(" Size:  ", totalKeys, "  (class ", probClass, ")");
  writeln(" Iterations:   ", maxIterations, "\n");

  // Generate Keys
  for i in D {
    randomStream.fillRandom(tempreals);
    keyArray(i) = (max * (+ reduce tempreals)):int;
  }

  // Do one iteration for free (i.e., untimed) to guarantee initialization of
  // all data and code pages and respective tables
  rank(1);

  if (probClass != classVals.S) then writeln("   iteration");

  time.start();
  for i in 1..maxIterations {
    if (probClass != classVals.S) then writeln("        ", i);
    rank(i);
  }
  time.stop();
  fullVerify();

  writeln("\n\n IS Benchmark Completed");
  writeln(" Class           = ", probClass);
  writeln(" Size            = ", totalKeys);
  writeln(" Iterations      = ", maxIterations);
  if printTime {
    writeln(" Time in seconds = ", time.elapsed(TimeUnits.seconds));
    writeln(" Mop/s total     = ",
            (maxIterations*totalKeys)/time.elapsed(TimeUnits.seconds)/1000000);
  }
  if (passedVerifications == (maxIterations+1) * 5 + 1) then
    writeln(" Verification    = SUCCESSFUL");
  else
    writeln(" Verification    = FAILED ", passedVerifications);
} 


proc setupPartialVerify() {
  // Would like to use an array over an enumerated domain of arrays instead
  select probClass {
    when classVals.S do {
      testIndexArray = S_testIndexArray;
      testRankArray  = S_testRankArray;
    }
    when classVals.W do {
      testIndexArray = W_testIndexArray;
      testRankArray  = W_testRankArray; 
    }
    when classVals.A do {
      testIndexArray = A_testIndexArray;
      testRankArray  = A_testRankArray; 
    }
    when classVals.B do {
      testIndexArray = B_testIndexArray;
      testRankArray  = B_testRankArray; 
    }
    when classVals.C do {
      testIndexArray = C_testIndexArray;
      testRankArray  = C_testRankArray; 
    }
  }
}

proc rank(iteration: int) {
  const blkSize = totalKeys / nThreads;

  keyArray(iteration) = iteration;
  keyArray(iteration+maxIterations) = maxKey - iteration;

  partialVerifyVals = keyArray(testIndexArray);

  coforall thread in 0..nThreads-1 {
    const low = thread * blkSize;
    const high = if thread == nThreads-1 then D.high else (thread+1)*blkSize-1;
    keyBuff1(0..maxKey-1, thread) = 0;
    for k in keyArray(low..high) do
      keyBuff1(k, thread) += 1;
    keyBuff1(0..maxKey-1, thread) = + scan keyBuff1(0..maxKey-1, thread);
  }
  partialVerification(iteration);
}


proc partialVerification(iteration: int) {
  for i in 0..4 {
    var k = partialVerifyVals(i);
    var val = + reduce keyBuff1(k-1, 0..nThreads-1);
    select probClass {
      when classVals.S do {
        if (i <= 2) {
          if (val != testRankArray(i)+iteration) {
            writeln("Failed partial verification: iteration ",
                 iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        } else {
          if (val != testRankArray(i)-iteration) {
            writeln("Failed partial verification: iteration ",
                 iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        }
      }
      when classVals.W do {
        if (i < 2) {
          if (val != testRankArray(i) + (iteration-2)) {
            writeln("Failed partial verification: iteration ",
                 iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        } else {
          if (val != testRankArray(i) - iteration) {
            writeln("Failed partial verification: iteration ",
                 iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        }
      }
      when classVals.A do {
        if (i <= 2) {
          if (val != testRankArray(i) + (iteration-1)) {
            writeln("Failed partial verification: iteration ",
                 iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        } else {
          if (val != testRankArray(i) - (iteration-1)) {
            writeln("Failed partial verification: iteration ",
                 iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        }
      }
      when classVals.B do {
        if (i == 1 || i == 2 || i == 4) {
          if (val != testRankArray(i) + iteration) {
            writeln("Failed partial verification: iteration ",
                 iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        } else {
          if (val != testRankArray(i) - iteration) {
            writeln("Failed partial verification: iteration ",
                 iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        }
      }
      when classVals.C do {
        if (i <= 2) {
          if (val != testRankArray(i) + iteration) {
            writeln("Failed partial verification: iteration ",
                 iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        } else {
          if (val != testRankArray(i) - iteration) {
            writeln("Failed partial verification: iteration ",
                 iteration, ", test key ", i);
          } else {
            passedVerifications += 1;
          }
        }
      }
    }
  }
}

proc fullVerify() {
  var failures = 0;
  for i in 0..maxKey-1 do
    keyBuff1(i, 0) = + reduce keyBuff1(i, 0..nThreads-1);
  buffer = keyArray;

  for i in D {
    keyBuff1(buffer(i), 0) -= 1;
    keyArray(keyBuff1(buffer(i), 0)) = buffer(i);
  }

  for i in 0..D.size-2 {
    if (keyArray(i) > keyArray(i+1)) then failures += 1;
  }

  if (failures != 0) then
    halt("Full verification: number of keys out of sort ", failures);
  else
    passedVerifications += 1;
}

