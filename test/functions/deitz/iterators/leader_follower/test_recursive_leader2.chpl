iter bar(lo: int, hi: int) {
  for i in lo..hi do
    yield i;
}

iter bar(param tag: iterator, lo: int, hi: int): int where tag == iterator.leader {
  yield lo;
  if hi > lo then
    for i in bar(tag, lo+1, hi) do
      yield i;
}

iter bar(param tag: iterator, follower, lo: int, hi: int) where tag == iterator.follower {
  yield follower;
}

config const n: int = 8;

forall i in bar(1, n) do
  writeln(i);
