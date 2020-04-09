class C {
  iter these(param tag: iterKind) where tag == iterKind.leader {
    yield 1..10;
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    for i in followThis do
      yield i;
  }
}

var myC1 = new C();
var myC2 = new C();

forall (i,j) in (myC1, myC2) {
  writeln("(i,j) = ", (i,j));
}
