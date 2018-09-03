//
// Test how long it takes to iterate over a non-strided array versus a
// strided array with the same number of elements
//

config const m = 1024;
config const n = 21457;
config param printTiming = false;

config const randSeed:int(64) = 314159265;

proc main {
  const D1 = {1..#m*n};
  const D2 = {1.. by 1 # m*n};
  const D3 = {1..#m, 1..#n};
  const D4 = {1.. by 1 #m, 1.. by 1 # n};

  var alpha: real = 3.14;
  var t1, t2, t3, t4: real;

  t1 = runTest(D1, alpha);
  t2 = runTest(D2, alpha);
  t3 = runTest(D3, alpha);
  t4 = runTest(D4, alpha);
  if printTiming {
    writeln("One Dimensional         : ", t1, " milliseconds.");
    writeln("One Dimensional strided : ", t2, " milliseconds.");
    writeln("Two Dimensional         : ", t3, " milliseconds.");
    writeln("Two Dimensional strided : ", t4, " milliseconds.");
  }
}

proc initialize(B) {
  use Random;
  var rnd = new RandomStream(eltType=real, seed=randSeed);
  rnd.fillRandom(B);
}

proc runTest(D, alpha: real) {
  use Time;
  var timer: Timer;
  var A, B: [D] real;

  initialize(B);

  timer.start();
  for (a,b) in zip(A, B) {
    a = alpha * b;
  }
  timer.stop();
  return timer.elapsed(TimeUnits.milliseconds);
}
