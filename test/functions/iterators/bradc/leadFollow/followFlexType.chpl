class C {
  def these() {
    yield 0.0;
  }

  def these(param tag: iterator) where tag == iterator.leader {
    var D: domain(1, int(32)) = [1..10];
    yield D;
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    for i in follower do
      yield i:real;
  }
}

class D {
  def these() {
    yield 0.0;
  }

  def these(param tag: iterator) where tag == iterator.leader {
    var D: domain(1, int(64)) = [1..10:int(64)];
    yield D;
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    for i in follower do
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
