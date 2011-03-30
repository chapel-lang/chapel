proc bar(i: int) return [1..3];

var A = bar(1..3);

iter foo() {
  for i in 1..3 do
    for j in A(i) do
      yield j;
}

iter foo(param tag: iterator) where tag == iterator.leader {
  for i in 1..3 do
    yield A(i);
}

iter foo(param tag: iterator, follower) where tag == iterator.follower {
  for j in follower do
    yield j;
}

for i in foo() do
  writeln(i);
writeln();
forall i in foo() do
  writeln(i);

