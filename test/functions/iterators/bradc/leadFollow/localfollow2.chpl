class C {
  def these() {
    for i in 1..10 do
      yield i: real;
  }

  def these(param tag: iterator) where tag == iterator.leader {
    yield [1..3];
    yield [4..7];
    yield [8..10];
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    writeln("follower is: ", follower);
    local {
      for i in follower do
        yield i: real;
    }
    yield 0.0;
  }
}

var myC = new C();

forall i in myC {
  writeln("i is: ", i);
}
