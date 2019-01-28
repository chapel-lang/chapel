/*
This test combines two aspects of reductions:

* plus-reduction of booleans - this implicitly coerces booleans to integers
  this is a simplification of test/trivial/deitz/monte.chpl

* test2 and test3 are testing simply reductions over ints,
  with promotion or not

* reduction can be over a serial, standalone, or leader-follower iterators

All these are probably tested elsewhere.
I am combined them here as a simpler program for use in debugging.
*/

config var n = 4;
config var m = 10;

config param useSA = false; // use the standalone iterator?
config param useLF = false; // use leader-follower?

// Part 1: reduction using a reduce intent.
// This is easier to deal with because the user declares the type
// of the variable subject to the reduce intent.

iter randomBoolPoints() {
  writeln("randomBoolPoints serial iterator");
  for i in 1..n {
    yield i <= m;
  }
}

iter randomBoolPoints(param tag) where tag == iterKind.standalone && useSA
{
  writeln("randomBoolPoints standalone iterator");
  for i in 1..n {
    yield i <= m;
  }
}

iter randomBoolPoints(param tag) where tag == iterKind.leader && useLF
{
  writeln("randomBoolPoints leader iterator");
  yield 1..n;
}

iter randomBoolPoints(followThis, param tag)
  where tag == iterKind.follower && useLF
{
  writeln("randomBoolPoints follower iterator");
  for i in followThis {
    yield i <= m;
  }
}

proc test1 {
  // skip 'forall' unless LF or SA are enabled
  if useLF || useSA {
    var result: int;
    forall idx in randomBoolPoints() with (+ reduce result) {
      result += idx;
    }
    writeln(result);
  }
}

// Part 2: reduction using Chapel's reduce expression.
// The compiler must infer the result type of the reduction.

iter randomIntPoints() {
  writeln("randomIntPoints serial iterator");
  for i in 1..n {
    yield i;
  }
}

iter randomIntPoints(param tag) where tag == iterKind.standalone && useSA
{
  writeln("randomIntPoints standalone iterator");
  for i in 1..n {
    yield i;
  }
}

iter randomIntPoints(param tag) where tag == iterKind.leader && useLF
{
  writeln("randomIntPoints leader iterator");
  yield 1..n;
}

iter randomIntPoints(followThis, param tag)
  where tag == iterKind.follower && useLF
{
  writeln("randomIntPoints follower iterator");
  for i in followThis {
    yield i;
  }
}

proc d2(x: numeric) return x**2;

proc test2 {
  const result = + reduce randomIntPoints();
  writeln(result);
}

proc test3 {
  const result = + reduce d2(randomIntPoints());
  writeln(result);
}

proc test4 {
  const result = + reduce (randomIntPoints() <= m);
  writeln(result);
}

proc main {
  test1;
  test2;
  test3;
  test4;
}
