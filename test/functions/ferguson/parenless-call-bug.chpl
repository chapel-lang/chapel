// Bug reproducer from issue #25266
// This code is based upon the test
//  parallel/forall/may-vs-must-par/promo-use-standalone.chpl

config const ni = 2;
const RNGi = 1..ni;

var count: atomic int;
proc inc1i { count.add(1); }

iter iter1par() {
  writeln("iter1par serial");
  for idx in RNGi do
    yield idx;
}
iter iter1par(param tag) where tag == iterKind.leader {
  writeln("iter1par leader");
  for idx in RNGi.these(tag) do
    yield idx;
}
iter iter1par(param tag, followThis) where tag == iterKind.follower {
  inc1i; // bug: this call was generating an incorrect error
  for idx in RNGi.these(tag, followThis) do
    yield idx;
}

proc multiply(arg1: int, arg2: int)   { writeln("mul=", arg1*arg2); }

multiply([ IND in iter1par() ] IND, 0);

assert(count.read() > 0); // make sure inc1i was run
