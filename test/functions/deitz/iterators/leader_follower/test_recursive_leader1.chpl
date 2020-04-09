iter foo(lo: int, hi: int) {
  for i in lo..hi do
    yield i;
}

iter foo(param tag: iterKind, lo: int, hi: int) where tag == iterKind.leader {
  coforall i in lo..hi do
    yield i;
}

iter foo(param tag: iterKind, followThis, lo: int, hi: int) where tag == iterKind.follower {
  yield followThis;
}

iter bar(lo: int, hi: int) {
  for i in lo..hi do
    yield i;
}

iter bar(param tag: iterKind, lo: int, hi: int): int where tag == iterKind.leader {
  if lo < hi {
    var mid = (lo + hi) / 2;
    cobegin {
      for i in bar(tag, lo, mid) do
        yield i;
      for i in bar(tag, mid+1, hi) do
        yield i;
    }
  } else if lo == hi then {
    yield lo;
  }
}

iter bar(param tag: iterKind, followThis, lo: int, hi: int) where tag == iterKind.follower {
  yield followThis;
}

config const n: int = 8;

var A: [1..n] int;

writeln("foo");
forall i in foo(1, n) do
  A(i) = i;

writeln(A);

A = 0;

writeln("bar");
forall i in bar(1, n) do
  A(i) = i;

writeln(A);
