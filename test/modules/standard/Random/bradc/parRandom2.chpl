use Random;

config var n = 10000:int(64);

const D = [1..n];

var A = new C();
var B: [D] real;

var randStr1 = new RandomStream(314159265);
var randStr2 = new RandomStream(314159265);

forall (a,r) in (A, randStr1) do
  a = r;

for b in B do
  b = randStr2.getNext();

for (i,a,b) in (D,A,B) {
  if (a != b) then
    writeln("mismatch at #", i, ": ", a, " != ", b);
  else
    writeln("#", i, " = ", a);
}

class C {
  var A: [1..n] real;

  iter these() {
    for i in 1..n do
      yield A(i);
  }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    cobegin {
      yield [n/2+1..n-1];
      yield [0..n/2];
    }
  }

  iter these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
    for i in followThis do
      yield A(i+1);
  }

  proc this(i) ref {
    return A(i);
  }
}
