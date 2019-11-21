config const timing = true;
config const perf = false;
config const correctness = false;
config const table = false;

use Time;
use Random;

var rng = createRandomStream(eltType=int, seed=0, parSafe=false, algorithm=RNG.PCG);

//var t2 = new Timer();
//var t3 = new Timer();
//var size1, size2, size3 = 0;

// iters, size, num_domains
var configs = [
               (1000, 10000, 1),
               (1000, 10000, 7),
               (1000, 10000, 123),
               (10, 1000000, 1),
               (10, 1000000, 7),
               (10, 1000000, 123)
              ];
if table {
  for (cfg,i) in zip(configs, 1..) {
    var (iters, size, num_domains) = cfg;

    if correctness then iters = 1;

    writeln(i, ": iters=", iters, " size=", size, " num_domains=", num_domains);
  }

  writeln();

  for (cfg,idx) in zip(configs, 1..) {
    if idx != 1 then write("\t");
    writef("% 6i", idx);
  }
  writeln();

}

for (cfg,idx) in zip(configs, 1..) {
  var (iters, size, num_domains) = cfg;

  if correctness then iters = 1;

  var t1 = new Timer();

  t1.start();

  for it in 1..iters {
    var D1:[1..num_domains] domain(int, parSafe=false);

    // Doesn't work with tuple?
    for i in 1..size {
      var j = rng.getNext(1, num_domains);
      D1(j) += i;
    }

    for j in 1..num_domains {
      D1(j).clear();
    }
  }

  t1.stop();

  if timing {
    if perf {
      writef("%i: % 6.3r\n", idx, t1.elapsed());
    } else {
      if idx != 1 then write("\t");
      writef("% 6.3r", t1.elapsed());
    }
  }
}

if timing {
  writeln();
}


if perf || correctness {
  writeln("SUCCESS");
}
