class C {
  iter these() {
    for i in 1..10 do
      yield i: real;
  }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    yield {1..3};
    yield {4..7};
    yield {8..10};
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    writeln("followThis is: ", followThis);
    local {
      for i in followThis do
        yield i: real;
    }
    yield 0.0;
  }
}

var ownMyC = new owned C();
var myC = ownMyC.borrow();

forall i in myC {
  writeln("i is: ", i);
}
