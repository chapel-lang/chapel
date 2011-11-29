iter foo(n: int) {
  for i in 1..n do
    yield i;
}

iter foo(n: int, param tag: iterKind) where tag == iterKind.leader {
  writeln("running leader iterator");
  cobegin {
    yield 0..n/2-1;
    yield n/2..n-1;
  }
}

iter foo(n: int, followThis, param tag: iterKind) where tag == iterKind.follower{
  for i in followThis+1 do
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
