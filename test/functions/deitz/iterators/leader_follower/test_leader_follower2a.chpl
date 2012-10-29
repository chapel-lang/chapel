iter foo(n: int) {
  for i in 1..n do
    yield i;
}

iter foo(param tag: iterKind, n: int) where tag == iterKind.leader {
  cobegin {
    yield 0..3;
    yield 5..n-1;
  }
}

iter foo(param tag: iterKind, followThis, n: int) where tag == iterKind.follower {
  for i in followThis+1 do
    yield i;
}

iter bar(n: int) {
  for i in 1..n do
    yield i;
}

iter bar(param tag: iterKind, n: int) where tag == iterKind.leader {
  yield 0..2;
  yield 4..n-1;
}

iter bar(param tag: iterKind, followThis, n: int) where tag == iterKind.follower {
  for i in followThis+1 do
    yield i;
}


config var n: int = 8;

var A, B: [1..n] int;

for (i,j) in zip(foo(n), bar(n)) {
  A(i) = 1;
  B(j) = 1;
}

writeln(A);

forall (i,j) in zip(foo(n), bar(n)) {
  A(i) = 2;
  B(j) = 2;
}

writeln(A);

forall (i,j) in zip(bar(n), foo(n)) {
  A(i) = 3;
  B(j) = 3;
}

writeln(A);
