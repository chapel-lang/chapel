class C {
  var n: int;
  var A: [1..n] int;

  def these() var {
    for e in A do
      yield e;
  }

  def these(param tag: iterator) where tag == iterator.leader {
    writeln("leader method invoked");
    cobegin {
      yield 1..n/2;
      yield n/2+1..n;
    }
  }

  def these(param tag: iterator, follower) var where tag == iterator.follower {
    for i in follower do
      yield A(i);
  }
}

var c = new C(12);

writeln(c);

for e in c do
  e = 1;

writeln(c);

forall e in c do
  e = 2;

writeln(c);
