class C {
  iter these() {
    for i in 1..10 do
      yield i;
  }

  iter these(param tag: iterator) where tag == iterator.leader {
    yield 1..10;
  }

  iter these(param tag: iterator, follower) where tag == iterator.follower {
    for i in follower do
      yield i;
  }
}

class D {
  // BLC: Really, I'd prefer not to define this one either, but parNoSolo.chpl
  // already captures this case
  iter these() {
    for i in 1..10 do
      yield i;
  }
  //

  iter these(param tag: iterator, follower) where tag == iterator.follower {
    for i in follower do
      yield -i;
  }
}

var myC = new C();
var myD = new D();

forall (i,j) in (myC, myD) {
  writeln("(i,j) = ", (i,j));
}

delete myC;
delete myD;
