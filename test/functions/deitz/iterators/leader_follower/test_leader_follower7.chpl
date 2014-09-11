class C {
  var n: int;
  var A: [1..n] int;

  iter these() ref {
    for e in A do
      yield e;
  }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    writeln("leader method invoked");
    cobegin {
      yield 1..n/2;
      yield n/2+1..n;
    }
  }

  iter these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
    for i in followThis do
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

forall (e,f) in zip(c, forall f in c do f) do
  e = f + 1;

writeln(c);

delete c;
