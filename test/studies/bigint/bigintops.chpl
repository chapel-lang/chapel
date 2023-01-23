use BigInteger, Time, List;

config param perfTest = true;

config const numOps = 10;

proc main() {
  var inputSizes: [0..2] bigint;
  var bitSizes  : [0..2] bigint;

  // 64 bit
  inputSizes[0] = 1:bigint;

  // 128 bit
  inputSizes[1] = inputSizes[0] << 127;

  // 1024 bit
  inputSizes[2] = inputSizes[0] << 1023;

  for inputSize in inputSizes do
    runOpTest(inputSize);
  for inputSize in inputSizes do
    runAssignTest(inputSize);
  for inputSize in inputSizes do
    runOnStmtTest(inputSize);
}

// Case 1: operator
proc runOpTest(inputVal: bigint) {
  var a: bigint = inputVal;
  var b: bigint = inputVal;
  var c: bigint;

  var t: stopwatch;
  if perfTest then t.start();
  
  for i in 1..numOps do
    c = a + b;

  if perfTest {
    t.stop();
    writeln("Elapsed ", inputVal.sizeInBase(2), " op time = ", t.elapsed(TimeUnits.seconds));
  } else {
    writeln("Result: ", c);
  }
}

// Case 2: assignment operator
proc runAssignTest(inputVal: bigint) {
  var a: bigint = inputVal;
  var b: bigint;
  
  var t: stopwatch;
  if perfTest then t.start();
  
  for i in 1..numOps do
    b += a;

  if perfTest {
    t.stop();
    writeln("Elapsed ", inputVal.sizeInBase(2), " assign time = ", t.elapsed(TimeUnits.seconds));
  } else {
    writeln("Result: ", b);
  }
}

// Case 3: operate on self
proc runOnStmtTest(inputVal: bigint) {
  var a: bigint = inputVal;
  var b: bigint = 0;

  var t: stopwatch;
  if perfTest then t.start();

  for i in 1..numOps {
    b = 0;
    b.pow(a, 2:uint);
  }

  if perfTest {
    t.stop();
    writeln("Elapsed ", inputVal.sizeInBase(2), " on stmt time = ", t.elapsed(TimeUnits.seconds));
  } else {
    writeln("Result: ", b);
  }
}
