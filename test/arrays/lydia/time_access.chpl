proc assign(A, n: int(32)) {
  forall i in 0..#n {
    for j in 0..#n {
      A(i, j) = (i+j):int(32);
    }
  }
}

proc assign2(A, n: int(32)) {
  forall i in 0..#n {
    for j in 0..#n {
      A[i][j] = (i+j):int(32);
    }
  }
}

proc touch(A, n: int(32)): int {
  var k: int(32) = 0;
  for i in 0..#n {
    for j in 0..#n {
      k += A(i, j);
    }
  }
  return k;
}

proc touch2(A, n: int(32)): int {
  var k: int(32) = 0;
  for i in 0..#n {
    for j in 0..#n {
      k += A[i][j];
    }
  }
  return k;
}

config const arrsize: int(32) = 1000;
config const ntrials = 100;

extern proc c_trial();

config param compareTimes = false;

proc main {
  use Time;
  var A: [0..#arrsize, 0..#arrsize] int(32);
  var B: [0..#arrsize][0..#arrsize] int(32);
  var t, t2: Timer;
  var worked1, worked2 = true;
  assign(A, arrsize);
  assign2(B, arrsize);

  t.start();
  for i in 1..ntrials {
    if (touch(A, arrsize) != 999000000) {
      worked1 = false;
    }
  }
  t.stop();
  t2.start();
  for i in 1..ntrials {
    if (touch2(B, arrsize) != 999000000) {
      worked2 = false;
    }
  }
  t2.stop();

  if (!worked1) {
    writeln("Error, Chapel multidimensional array miscalculation!");
  }
  if (!worked2) {
    writeln("Error, Chapel array of arrays miscalculation!");
  }

  if compareTimes {
    writeln("chpl accessed [", arrsize, " ", arrsize, "] elements ", ntrials,
            " times in ", t.elapsed(TimeUnits.milliseconds)/1000, " seconds");
    writeln("chpl accessed [", arrsize, "][", arrsize, "] elements ", ntrials,
            " times in ", t2.elapsed(TimeUnits.milliseconds)/1000, " seconds");
    c_trial();
  }
}
