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
      on Locales(0) do yield 1..n/2;
      on Locales(1) do yield n/2+1..n;
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

for i in 1..c.n do
  c.A[i] = i;

writeln(c);

forall e in c do
  e = here.id;

writeln(c);
