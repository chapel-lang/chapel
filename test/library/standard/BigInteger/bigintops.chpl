use BigInteger, Time, List;

config param perfTest = true;

config const numOps = 10;

proc main() {
  var inputSizes: [0..3] bigint;

  // 64 bit
  inputSizes[0] = 1:bigint << 63;

  // 128 bit
  inputSizes[1] = 1:bigint << 127;

  // 256 bit
  inputSizes[2] = 1:bigint << 255;

  // 1024 bit
  inputSizes[3] = 1:bigint << 1023;

  for inputSize in inputSizes do
    runOpTest(inputSize);
  for inputSize in inputSizes do
    runAddAssignTest(inputSize);
  for inputSize in inputSizes do
    runPowTest(inputSize);
}

// Case 1: addition operator
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
    writeln("Elapsed ", inputVal.sizeInBase(2), " addition time = ", t.elapsed(TimeUnits.seconds));
  } else {
    writeln("Result: ", c);
  }
}

// Case 2: += operator
proc runAddAssignTest(inputVal: bigint) {
  var a: bigint = inputVal;
  var b: bigint;
  
  var t: stopwatch;
  if perfTest then t.start();
  
  for i in 1..numOps do
    b += a;

  if perfTest {
    t.stop();
    writeln("Elapsed ", inputVal.sizeInBase(2), " add assign time = ", t.elapsed(TimeUnits.seconds));
  } else {
    writeln("Result: ", b);
  }
}

// Case 3: operate on self
proc runPowTest(inputVal: bigint) {
  var a: bigint = inputVal;
  var b: bigint = 0;

  var t: stopwatch;
  if perfTest then t.start();

  for i in 1..numOps {
    b.pow(a, 2:uint);
  }

  if perfTest {
    t.stop();
    writeln("Elapsed ", inputVal.sizeInBase(2), " pow time = ", t.elapsed(TimeUnits.seconds));
  } else {
    writeln("Result: ", b);
  }
}
