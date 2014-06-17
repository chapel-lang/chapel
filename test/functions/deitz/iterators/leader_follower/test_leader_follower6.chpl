iter foo(n: int) {
  for i in 1..n do
    yield i;
}

iter foo(param tag: iterKind, n: int) where tag == iterKind.leader {
  cobegin {
    yield 0..n/2-1;
    yield n/2..n-1;
  }
}

iter foo(param tag: iterKind, followThis, n: int) where tag == iterKind.follower {
  for i in followThis+1 do
    yield i:int;
}

iter bar(n: uint) {
  for i in 1..n do
    yield i;
}

iter bar(param tag: iterKind, n: uint) where tag == iterKind.leader {
  yield 0..n/2-2;
  yield n/2-1..n-1;
}

iter bar(param tag: iterKind, followThis, n: uint) where tag == iterKind.follower {
  for i in followThis+1 do
    yield i:uint;
}


config var n: int = 8;

var A: [1..n] int;
var B: [1..n:uint] int;

for (i,j) in zip(foo(n), bar(n:uint)) {
  A(i) = 1;
  B(j) = 1;
}

writeln(A);

forall (i,j) in zip(foo(n), bar(n:uint)) {
  A(i) = 2;
  B(j) = 2;
}

writeln(A);

forall (i,j) in zip(foo(n), bar(n:uint)) {
  A(i) = 3;
  B(j) = 3;
}

writeln(A);
