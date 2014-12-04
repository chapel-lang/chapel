config const n = 100000;
config const showRace = false;

proc test(param width, n_max:int ) {
  var n:uint(width) = max(uint(width));
  n -= 1; // don't go to max(type) since that will overflow the range iterator
  if n_max:uint(64) < n:uint(64) then n = n_max:uint(width);

  const mult:uint(width) = 10;
  const one:uint(width) = 1;
  const zero:uint(width) = 0;

  writeln("Testing " + width:string + " - bit atomics");

  if showRace {
    var x:uint(width);
    x = zero;
    forall i in one..n with (ref x) { // race is intentional
      x += one;
    }
    writeln("X is ", x," (vs. ", n, ")");
  }

  var aint:atomic uint(width);

  //aint.init(0);

  forall i in one..n {
    aint.fetchAdd(one);
  }

  assert(aint.read() == n);
  writeln("Increment OK");

  forall i in one..n {
    aint.fetchSub(one);
  }

  assert(aint.read() == zero);

  writeln("Decrement OK");

  forall i in one..n {
    aint.fetchAdd(mult);
  }

  assert(aint.read() == (mult*n):uint(width));
  writeln("Add OK");

  forall i in one..n {
    aint.fetchSub(mult);
  }

  assert(aint.read() == zero);
  writeln("Subtract OK");

  aint.compareExchangeStrong(zero, mult);

  assert(aint.read() == mult);

  writeln("Compare and Swap OK");

  //aint.destroy();
}

test(8,  n);
test(16, n);
test(32, n);
//test(64, n);
