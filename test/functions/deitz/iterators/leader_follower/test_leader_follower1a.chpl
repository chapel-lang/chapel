def foo(n: int) {
  for i in 1..n do
    yield i;
}

def foo(leader, n: int) {
  cobegin {
    yield 0..n/2-1;
    yield n/2..n-3;
  }
}

def foo(follower, n: int) {
  for i in follower+2 do
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
