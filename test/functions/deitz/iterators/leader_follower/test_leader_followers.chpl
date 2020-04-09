iter foo(n: int) {
  for i in 1..n do
    yield i;
}

iter foo(param tag: iterKind, n: int) where tag == iterKind.leader {
  yield 0..n-1;
}

iter foo(param tag: iterKind, followThis, n: int) where tag == iterKind.follower {
  writeln("foo range follower called");
  for i in followThis+1 do
    yield i;
}

iter bar(n: int) {
  for i in 1..n do
    yield i;
}

iter bar(param tag: iterKind, n: int) where tag == iterKind.leader {
  yield n-1;
}

iter bar(param tag: iterKind, followThis: range, n: int) where tag == iterKind.follower {
  writeln("bar range follower called");
  for i in followThis+1 do
    yield i;
}

iter bar(param tag: iterKind, followThis: int, n: int) where tag == iterKind.follower {
  writeln("bar int follower called");
  for i in 1..followThis+1 do
    yield i;
}

forall i in zip(foo(5), bar(5)) do
  writeln(i);

forall i in zip(bar(5), bar(5)) do
  writeln(i);
