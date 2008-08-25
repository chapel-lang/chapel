def foo(n: int) {
  for i in 1..n do
    yield i;
}

def foo(leader, n: int) {
  cobegin {
    yield 0..3;
    yield 5..n-1;
  }
}

def foo(follower, n: int) {
  for i in follower+1 do
    yield i;
}

def bar(n: int) {
  for i in 1..n do
    yield i;
}

def bar(leader, n: int) {
  yield 0..2;
  yield 4..n-1;
}

def bar(follower, n: int) {
  for i in follower+1 do
    yield i;
}


config var n: int = 8;

var A, B: [1..n] int;

for (i,j) in (foo(n), bar(n)) {
  A(i) = 1;
  B(j) = 1;
}

writeln(A);

forall (i,j) in (foo(n), bar(n)) {
  A(i) = 2;
  B(j) = 2;
}

writeln(A);

forall (i,j) in (bar(n), foo(n)) {
  A(i) = 3;
  B(j) = 3;
}

writeln(A);
