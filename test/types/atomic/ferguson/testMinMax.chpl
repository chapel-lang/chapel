use Random;

config const n = 100_000;
config const verbose = false;

proc testSimple(type t) {
  const zero = 0:t;
  const one  = 1:t;
  const two  = 2:t;
  const many = 100:t;

  var myMin: atomic t = many;
  assert(myMin.read() == many);
  assert(myMin.fetchMin(many) == many);
  assert(myMin.read() == many);
  assert(myMin.fetchMin(two) == many);
  assert(myMin.read() == two);
  assert(myMin.fetchMin(one) == two);
  assert(myMin.read() == one);
  assert(myMin.fetchMin(two) == one);
  assert(myMin.read() == one);
  assert(myMin.fetchMin(zero) == one);
  assert(myMin.read() == zero);

  // check also non-fetching min
  myMin.write(many);
  assert(myMin.read() == many);
  myMin.min(many);
  assert(myMin.read() == many);
  myMin.min(two);
  assert(myMin.read() == two);
  myMin.fetchMin(one);
  assert(myMin.read() == one);
  myMin.min(two);
  assert(myMin.read() == one);
  myMin.min(zero);
  assert(myMin.read() == zero);

  var myMax: atomic t = zero;
  assert(myMax.read() == zero);
  assert(myMax.fetchMax(zero) == zero);
  assert(myMax.read() == zero);
  assert(myMax.fetchMax(one) == zero);
  assert(myMax.read() == one);
  assert(myMax.fetchMax(two) == one);
  assert(myMax.read() == two);
  assert(myMax.fetchMax(one) == two);
  assert(myMax.read() == two);
  assert(myMax.fetchMax(many) == two);
  assert(myMax.read() == many);

  // check also non-fetching max
  myMax.write(zero);
  assert(myMax.read() == zero);
  myMax.max(zero);
  assert(myMax.read() == zero);
  myMax.max(one);
  assert(myMax.read() == one);
  myMax.max(two);
  assert(myMax.read() == two);
  myMax.max(one);
  assert(myMax.read() == two);
  myMax.max(many);
  assert(myMax.read() == many);
}

proc testNonFetchingMin(type t, Input: [] t) {
  // use the non-fetching min version
  var myMin : atomic t = Input[0];
  forall x in Input {
    myMin.min(x);
  }
  assert(myMin.read() == min reduce Input);
}

proc testFetchingMin(type t, Input: [] t) {
  // use the fetching min version
  var myMin : atomic t = Input[0];
  forall x in Input {
    myMin.fetchMin(x);
  }
  assert(myMin.read() == min reduce Input);
}

proc testNonFetchingMax(type t, Input: [] t) {
  // use the non-fetching min version
  var myMax : atomic t = Input[0];
  forall x in Input {
    myMax.max(x);
  }
  assert(myMax.read() == max reduce Input);
}

proc testFetchingMax(type t, Input: [] t) {
  // use the fetching max version
  var myMax : atomic t = Input[0];
  forall x in Input {
    myMax.fetchMax(x);
  }
  assert(myMax.read() == max reduce Input);
}

proc testMinMaxesForArray(type t, Input: [] t) {
  testNonFetchingMin(t, Input);
  testFetchingMin(t, Input);
  testNonFetchingMax(t, Input);
  testFetchingMax(t, Input);
}

proc testMinMaxes(type t) {
  writeln("testMinMaxes(", t:string, ")");

  testSimple(t);

  var Up: [0..<n] t = [i in 0..<n] i:t;
  var Down: [0..<n] t = [i in 0..<n by -1] i:t;
  var Random: [0..<n] t;

  fillRandom(Random, seed=121);

  testMinMaxesForArray(t, Up);
  testMinMaxesForArray(t, Down);
  testMinMaxesForArray(t, Random);
}

proc main() {
  testMinMaxes(int(8));
  testMinMaxes(int(16));
  testMinMaxes(int(32));
  testMinMaxes(int(64));
  testMinMaxes(uint(8));
  testMinMaxes(uint(16));
  testMinMaxes(uint(32));
  testMinMaxes(uint(64));

  testMinMaxes(real(32));
  testMinMaxes(real(64));
}
