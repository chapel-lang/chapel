class C {
  def these(leader) {
    yield 1..10;
  }

  def these(follower) {
    for i in follower do
      yield i;
  }
}

var myC1 = new C();
var myC2 = new C();

forall (i,j) in (myC1, myC2) {
  writeln("(i,j) = ", (i,j));
}
