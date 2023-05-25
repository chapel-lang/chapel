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

var myC = (new owned C()).borrow();
var myD = (new owned D()).borrow();

forall (i,j) in zip(myC, myC) do
  writeln("(i,j) = ", (i,j));
writeln();

forall (i,j) in zip(myD, myD) do
  writeln("(i,j) = ", (i,j));
writeln();

forall (i,j) in zip(myC,myD) do
  writeln("(i,j) = ", (i,j));
writeln();

forall (i,j) in zip(myD,myC) do
  writeln("(i,j) = ", (i,j));
writeln();
