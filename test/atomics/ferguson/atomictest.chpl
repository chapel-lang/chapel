config const n:uint(32) = 100000;
config const showRace = false;
const mult:uint(32) = 10;
const one:uint(32) = 1;

use Atomics;


if showRace {
  var x:uint(32);
  x = 0;
  forall i in one..n {
    x += one;
  }
  writeln("X is ", x," (vs. ", n, ")");
}

var aint:atomic_uint32;

aint.init(0);

forall i in one..n {
  aint.fetchAdd(one);
}

assert(aint.load() == n);
writeln("Increment OK");

forall i in one..n {
  aint.fetchSub(one);
}

assert(aint.load() == 0);

writeln("Decrement OK");

forall i in one..n {
  aint.fetchAdd(mult);
}

assert(aint.load() == mult*n);
writeln("Add OK");

forall i in one..n {
  aint.fetchSub(mult);
}

assert(aint.load() == 0);
writeln("Subtract OK");

aint.compareExchangeStrong(0, mult);

assert(aint.load() == mult);

writeln("Compare and Swap OK");

aint.destroy();

