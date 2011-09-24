config const n = 100000;
config const showRace = false;

use Atomics;


if showRace {
  var x:int;
  x = 0;
  forall i in 1..n {
    x += 1;
  }
  writeln("X is ", x," (vs. ", n, ")");
}

var aint:atomicuint;

aint.set(0);

forall i in 1..n {
  aint.increment();
}

assert(aint.read() == n);
writeln("Increment OK");

forall i in 1..n {
  aint.decrement();
}

assert(aint.read() == 0);

writeln("Decrement OK");

forall i in 1..n {
  aint.add(10);
}

assert(aint.read() == 10*n);
writeln("Add OK");

forall i in 1..n {
  aint.subtract(10);
}

assert(aint.read() == 0);
writeln("Subtract OK");

aint.compareAndSwap(0, 10);

assert(aint.read() == 10);

writeln("Compare and Swap OK");
