/*
Dense dot product performance testing intended for manual usage.
*/

use LinearAlgebra;
use BlockDist;
use Time;
use Math;

config param useBlockDist = false;

config const n=1024,
             iters=10,
             thresh=1.0e-10,
             /* Omit timing output */
             correctness=false;

config type eltType = real;

const nbytes = numBytes(eltType);

proc main() {
  const dom = {1..n, 1..n} dmapped if useBlockDist then
                      new dmap(new Block(boundingBox={1..n, 1..n})) else
                      defaultDist;
  
  var BA : [dom] real = [(i,j) in dom] (j / (1000000));

  var t: Timer;

  if !correctness {
    writeln('============================');
    writeln('Dense Dot Performance Test');
    writeln('============================');
    writeln('iters                  : ', iters);
    writeln('square matrix size     : ', n);
    writeln('MB                     : ', (nbytes*n*n) / 10**6);
    writeln();
  }

  t.start();
  var result = dot(BA, BA);
  t.stop();

  for 1..<iters {
    t.start();  
    result = dot(BA, BA);
    t.stop();
  }

  if correctness {
    var reference : [dom] real = 0;
    forall (i,j) in dom {
      reference[i,j] = + reduce (BA[i,..]*BA[..,j]); 
    }
    const diff = abs(+ reduce (result - reference));
    if diff > thresh {
      writeln("FAILED ", diff);
    }
    else {
      writeln("PASSED");
    }
  }

  if !correctness then
    writeln('Time: ', t.elapsed() / iters);
  t.clear();

}

