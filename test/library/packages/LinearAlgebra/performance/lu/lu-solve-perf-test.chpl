use Time;
use Math;
use LinearAlgebra;
use Random;

config const m=4,
             iters=10,
             /* Omit timing output */
             correctness=false;
             
config type eltType = real;

const nbytes = numBytes(eltType);

proc main() {

  const Aspace = {1..m, 1..m};
  const bspace = {1..m};
  const eps = 1e-7;

  var t: Timer;

  if !correctness {
    writeln('=========================================');
    writeln(' Solve Performance Test');
    writeln('=========================================');
    writeln('iters : ', iters);
    writeln('m     : ', m);
    writeln('numLocales     : ', numLocales);
    writeln('MB    : ', (nbytes*m*(m+1)) / 10**6);
    writeln();
  }

  var A = eye(m);
  var b : [1..m] real;
  var x: [1..m] eltType;

  fillRandom(b, seed=10);

  for 1..iters {
    t.start();
    x = solve(A, b);
    t.stop();
  }

  if !correctness then
    writeln('LinearAlgebra.solve:', t.elapsed()/iters);
  else{
      var y = dot(A, x);
      var totalDiff = + reduce(abs(b-y));
      if totalDiff > eps then
        writeln("Found incorrect solution");
  }

  t.clear();
}
