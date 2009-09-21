record R {
  var A: [1..3] int;
}

var AR: [1..3] R;

def foo() {
  for i in 1..3 do
    for j in AR(i).A do
      yield j;
}

def foo(param tag: iterator) where tag == iterator.leader {
  for i in 1..3 do
    yield AR(i);
}

def foo(param tag: iterator, follower) where tag == iterator.follower {
  for j in follower.A do
    yield j;
}

for i in foo() do
  writeln(i);
writeln();
forall i in foo() do
  writeln(i);

