// Test _grow performance

use LayoutCS;
use Time;

// TODO: shrink
enum Tests {grow, bulkGrow};
use Tests;

config const n = 1000,
             iters = 100,
             test = grow,
             correctness=false;

var t: Timer;

proc main() {
  if test == grow then
    testGrow();
  else if test == bulkGrow then
    testBulkGrow();
  else
    writeln('--test not recognized');

  if !correctness {
    writeln('***');
    writeln('Note: Time expressed in milliseconds');
    writeln('***');
  }
}

//
// ChapelDistribution.BaseSparseDomImpl._grow()
//
proc testGrow() {
  const D = {1..n, 1..n};
  for 1..iters {
    var subD: sparse subdomain(D) dmapped CS();
    t.start();
    for i in 1..n {
      subD += (i, i);
    }
    t.stop();
    assert(subD.size == n);
  }

  if !correctness {
    writeln('_grow()');
    writeln('factor    : ', arrayAsVecGrowthFactor);
    writeln('additions : ', n);
    writeln('iterations: ', iters);
    writeln('time/iter : ', (t.elapsed() / iters)*1000);
  }
}

//
// ChapelDistribution.BaseSparseDomImpl._bulkGrow()
//
proc testBulkGrow() {
  const D = {1..n, 1..n};
  var Diag: [1..n] 2*int;
  [i in 1..n] Diag[i] = (i,i);

  for 1..iters {
    var subD: sparse subdomain(D) dmapped CS();
    t.start();
    subD += Diag;
    t.stop();
    assert(subD.size == n);
  }


  if !correctness {
    writeln('_bulkGrow()');
    // Currently does not impact growth
    //writeln('factor    : ', arrayAsVecGrowthFactor); 
    writeln('grow size : ', n);
    writeln('iterations: ', iters);
    writeln('time/iter : ', (t.elapsed() / iters)*1000);
  }
}


