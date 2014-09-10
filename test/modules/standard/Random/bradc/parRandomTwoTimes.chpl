use Random;

config var n = 10;

const D = [1..2*n];

var A = new C();
var B: [D] real;
var myD = new DD();

var randStr1 = new RandomStream(314159265);
var randStr2 = new RandomStream(314159265);

forall (i,r) in (myD, randStr1) do
  A.A(i) = r;

forall (i,r) in (myD, randStr1) do
  A.B(i) = r;

for b in B do
  b = randStr2.getNext();

for (i,a,b) in (D,A,B) {
  if (a != b) then
    writeln("mismatch at #", i, ": ", a, " != ", b);
  else
    writeln("#", i, " = ", a);
}

class DD {
  iter these() {
    for i in 1..n do
      yield i;
  }

  iter these(leader) {
    cobegin {
      yield [n/2+1..n];
      yield [1..n/2];
    }
  }

  iter these(followThis) {
    for i in followThis do
      yield i;
  }
}

class C {
  var A: [1..n] real;
  var B: [1..n] real;

  iter these() {
    for i in 1..n do
      yield A(i);
    for i in 1..n do
      yield B(i);
  }

  iter these(leader) {
    cobegin {
      yield [((3*n)/2)+1..2*n];
      yield [n/2+1..n];
      yield [1..n/2];
      yield [n+1..(3*n)/2];
    }
  }

  iter these(followThis) ref {
    for i in followThis do
      if (i <= n) then
        yield A(i);
      else
        yield B(i-n);
  }

  proc this(i) ref {
    if (i <= n) then
      return A(i);
    else
      return B(i-n);
  }
}
