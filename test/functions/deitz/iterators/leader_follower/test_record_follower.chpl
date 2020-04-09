record R {
  var A: [1..3] int;
}

var AR: [1..3] R;

iter foo() {
  for i in 1..3 do
    for j in AR(i).A do
      yield j;
}

iter foo(param tag: iterKind) where tag == iterKind.leader {
  for i in 1..3 do
    yield AR(i);
}

iter foo(param tag: iterKind, followThis) where tag == iterKind.follower {
  for j in followThis.A do
    yield j;
}

for i in foo() do
  writeln(i);
writeln();
forall i in foo() do
  writeln(i);

