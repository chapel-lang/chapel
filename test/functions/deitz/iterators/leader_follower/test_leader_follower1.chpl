def foo(n: int) {
  for i in 1..n do
    yield i;
}

def foo(leader, n: int) {
  yield 0..n/2-1;
  yield n/2..n-1;
}

def foo(follower, n: int) {
  for i in follower+1 do
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
