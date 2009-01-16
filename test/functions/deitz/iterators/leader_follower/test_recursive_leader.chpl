def foo(n: int) {
  for i in 1..n do
    yield i;
}

def foo(param tag: iterator, n: int):range where tag == iterator.leader {
  for i in foo(iterator.leader, n/2) do
    yield i;
  yield n/2..n;
}

def foo(param tag: iterator, follower, n: int) where tag == iterator.follower {
  for i in follower do
    yield i;
}

config var n: int = 8;

var A: [1..n] int;

for i in foo(n) {
  A(i) = 1;
}

writeln(A);

forall i in foo(n) {
  A(i) = 2;
}

writeln(A);
