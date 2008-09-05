class C {
  var n: int;
  var A: [1..n] int;

  def these() var {
    for e in A do
      yield e;
  }

  def these(leader) {
    writeln("leader method invoked");
    cobegin {
      on Locales(0) do yield 1..n/2;
      on Locales(1) do yield n/2+1..n;
    }
  }

  def these(follower) var {
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

for i in 1..c.n do
  c.A[i] = i;

writeln(c);

forall e in c do
  e = here.id;

writeln(c);
