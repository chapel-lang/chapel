config param printTimes = false;
config param maxAdds = 5;
config const size = 100;
config const nTrials = 100;

proc arrayAccess(A: [] real, B: [] real, param nAdds) {
  const n = A.size;
  for 1..nTrials {
    for i in A.domain {
      for param adds in 1..nAdds {
        B[i] += A[i] + A[i];
      }
    }
  }
}

proc refAccess(A: [] real, B: [] real, param nAdds) {
  const n = A.size;
  for 1..nTrials {
    for i in A.domain {
      ref rA = A[i];
      for param adds in 1..nAdds {
        B[i] += rA + rA;
      }
    }
  }
}

proc main {
  use Time;
  var t1, t2: Timer;
  var Dom = {0..#size, 0..#size};
  var A: [Dom] real;
  var B: [Dom] real;
  var C: [Dom] real;

  for (i,j) in Dom do A[i,j] = i * 0.123456789 + j/0.123456789;

  t1.start();
  arrayAccess(A, B, maxAdds);
  t1.stop();

  t2.start();
  refAccess(A, C, maxAdds);
  t2.stop();

  for i in Dom do
    if B[i] != C[i] then
      halt("At index ", i, ": ", B[i], " != ", C[i]);

  if printTimes {
    writeln("Array access: ", t1.elapsed());
    writeln("Ref access: ", t2.elapsed());
  }
}

