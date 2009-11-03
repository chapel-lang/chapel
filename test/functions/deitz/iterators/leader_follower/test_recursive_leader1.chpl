def foo(lo: int, hi: int) {
  for i in lo..hi do
    yield i;
}

def foo(param tag: iterator, lo: int, hi: int) where tag == iterator.leader {
  coforall i in lo..hi do
    yield i;
}

def foo(param tag: iterator, follower, lo: int, hi: int) where tag == iterator.follower {
  yield follower;
}

def bar(lo: int, hi: int) {
  for i in lo..hi do
    yield i;
}

def bar(param tag: iterator, lo: int, hi: int): int where tag == iterator.leader {
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

def bar(param tag: iterator, follower, lo: int, hi: int) where tag == iterator.follower {
  yield follower;
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
