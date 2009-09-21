def bar(i: int) return [1..3];

var A = bar(1..3);

def foo() {
  for i in 1..3 do
    for j in A(i) do
      yield j;
}

def foo(param tag: iterator) where tag == iterator.leader {
  for i in 1..3 do
    yield A(i);
}

def foo(param tag: iterator, follower) where tag == iterator.follower {
  for j in follower do
    yield j;
}

for i in foo() do
  writeln(i);
writeln();
for i in foo() do
  writeln(i);

