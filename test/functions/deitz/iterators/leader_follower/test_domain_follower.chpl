proc bar(i: int) return {1..3};

// This promoted assignment creates an array of domains
// [ {1..3}, {1..3}, {1..3} ]
var A = bar(1..3);

iter foo() {
  for i in 1..3 do
    for j in A(i) do
      yield j;
}

iter foo(param tag: iterKind) where tag == iterKind.leader {
  for i in 1..3 do
    yield A(i);
}

iter foo(param tag: iterKind, followThis) where tag == iterKind.follower {
  for j in followThis do
    yield j;
}

for i in foo() do
  writeln(i);
writeln();
forall i in foo() do
  writeln(i);

