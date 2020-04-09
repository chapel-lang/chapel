/*
Dense dot product performance testing intended for manual usage.
*/

use LinearAlgebra;
use Time;
use BlockDist;
use Math;

config const n=1000000,
             iters=10,
             thresh=1.0e-10,
             /* Skip benchmarking against reduction */
             reference=false,
             /* Omit timing output */
             correctness=false;

config type eltType = real;

const nbytes = numBytes(eltType);

proc main() {
  const Space = {1..n};
  const BlockSpace = Space dmapped Block(boundingBox=Space);
  
  var BA : [BlockSpace] real = [i in BlockSpace] (i / (1000000));

  var t: Timer;

  if !correctness {
    writeln('============================');
    writeln('Dense Dot Performance Test');
    writeln('============================');
    writeln('iters : ', iters);
    writeln('n     : ', n);
    writeln('numLocales     : ', numLocales);
    writeln('MB    : ', (nbytes*n) / 10**6);
    writeln();
  }
  
  var d: eltType;

  for 1..iters {
    t.start();
    d = dot(BA, BA);
    t.stop();
  }
  
  if correctness {
    var d_reduce = + reduce (BA * BA);
    const diff = abs(d - d_reduce);
    if diff > thresh {
      writeln("FAILED ", diff);
    }
    else {
      writeln("PASSED");
    }
  }

  if !correctness then
    writeln('LinearAlgebra.dot: ', t.elapsed() / iters);
  t.clear();

  if reference {
    for 1..iters {
      t.start();
      d = + reduce (BA * BA);
      t.stop();
    }

    if !correctness then
      writeln('reduction: ', t.elapsed() / iters);
    t.clear();
  } else {
    if !correctness then
      writeln('reduction: -1');
  }
}

