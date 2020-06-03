use PrivatizationWrappers;
use Random;

config const n = 1000000;
config const m = 100;
config const reps = 10;
var A:[1..n] int;
fillRandom(A, seed=1, algorithm=RNG.PCG);
forall a in A {
  a = abs(a) % m;
  if a <= 1 then
    a = 2;
}

// First will store the element to initialize
// or 0 if we shouldn't initialize any element
var Filled:[1..m] int;
var First:[1..n] int;

for (a, first) in zip(A, First) {
  if Filled[a] == 0 then
    first = a;
  Filled[a] += 1;
}

var valueOne = new unmanaged C(1);
insertPrivatized(valueOne, 1);

var sum = 0;
for (i, first) in zip(A, First) {
  for j in 1..reps {
    // An appealing load for LICMing
    // It would even be OK for this program
    // to LICM this load.
    var one = getPrivatized(1);

    // This load might be less tempting to LICM
    // but tests that we aren't just relying on
    // an outdated copy of chpl_privateObjects.
    var c:unmanaged C?;
    if first == 0 || j > 1 {
      c = getPrivatized(i);
    } else {
      c = new unmanaged C(i);
      insertPrivatized(c!, i);
    }

    sum += c!.i + one.i;
  }
}

var expected = 0;
for (i, first) in zip(A, First) {
  for j in 1..reps {
    expected += i + 1;
  }
}

assert(sum == expected);

for i in A {
  var c = getPrivatized(i);
  if c != nil {
    delete c;
    clearPrivatized(i);
  }
}

delete valueOne;
