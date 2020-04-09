use Time;
config const n = 1000000;
config const showRace = false;

config const performance = false;

proc test(type base_type, n_max:int) {
  const mult = 10: base_type;
  const one  =  1: base_type;
  const zero =  0: base_type;

  var n:uint = max(base_type):uint;
  if isRealType(base_type) then n = max(uint);
  n = min(n, n_max:uint);

  // single precision floats have 24 bits of precision (i.e. can only
  // represent integers up to 2**24 exactly)
  if base_type == real(32) then n = min(n, (2**24)/mult:uint);

  // need to avoid signed integer overflow, since it's undefined behavior
  if isIntType(base_type) then
    if n > (max(base_type)/mult):uint then  n = n/mult:uint;


  writeln("Testing atomic " + base_type:string);

  if showRace {
    var x:base_type;
    x = zero;
    forall i in 1..n with (ref x) { // race is intentional
      x += one;
    }
    writeln("X is ", x," (vs. ", n, ")");
  }

  var aint:atomic base_type;

  var t: Timer;
  t.start();
  forall i in 1..n {
    aint.fetchAdd(one);
  }
  t.stop();
  if performance && n == (n_max:uint) then
    writeln("Elapsed seconds for fetchAdd on atomic ", base_type:string, ": ", t.elapsed());

  assert(aint.read() == n:base_type);
  writeln("Increment OK");


  forall i in 1..n {
    aint.fetchSub(one);
  }

  assert(aint.read() == zero);

  writeln("Decrement OK");

  forall i in 1..n {
    aint.fetchAdd(mult);
  }

  assert(aint.read() == (mult*n:base_type));
  writeln("Add OK");

  forall i in 1..n {
    aint.fetchSub(mult);
  }

  assert(aint.read() == zero);
  writeln("Subtract OK");

  aint.compareAndSwap(zero, mult);

  assert(aint.read() == mult);

  writeln("Compare and Swap OK");
  writeln();
}

test(uint(8),  n);
test(uint(16), n);
test(uint(32), n);
test(uint(64), n);

test(int(8),  n);
test(int(16), n);
test(int(32), n);
test(int(64), n);

test(real(32), n);
test(real(64), n);
