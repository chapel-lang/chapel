class C {
  iter these() {
    yield 0.0;
  }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    var D: domain(1, int(32)) = {1..10:int(32)};
    yield D;
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    for i in followThis do
      yield i:real;
  }
}

class D {
  iter these() {
    yield 0.0;
  }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    var D: domain(1, int(64)) = {1..10:int(64)};
    yield D;
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    for i in followThis do
      yield (-i):real;
  }
}

var myC = new C();
var myD = new D();

forall (i,j) in (myC, myC) do
  writeln("(i,j) = ", (i,j));
writeln();

forall (i,j) in (myD, myD) do
  writeln("(i,j) = ", (i,j));
writeln();

forall (i,j) in (myC,myD) do
  writeln("(i,j) = ", (i,j));
writeln();

forall (i,j) in (myD,myC) do
  writeln("(i,j) = ", (i,j));
writeln();

delete myC;
delete myD;
