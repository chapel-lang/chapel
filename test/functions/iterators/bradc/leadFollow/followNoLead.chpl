class C {
  def these() {
    for i in 1..10 do
      yield i;
  }

  def these(leader) {
    yield 1..10;
  }

  def these(follower) {
    for i in follower do
      yield i;
  }
}

class D {
  // BLC: Really, I'd prefer not to define this one either, but parNoSolo.chpl
  // already captures this case
  def these() {
    for i in 1..10 do
      yield i;
  }
  //

  def these(follower) {
    for i in follower do
      yield -i;
  }
}

var myC = new C();
var myD = new D();

forall (i,j) in (myC, myD) {
  writeln("(i,j) = ", (i,j));
}
