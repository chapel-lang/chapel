iter bar(lo: int, hi: int) {
  for i in lo..hi do
    yield i;
}

iter bar(param tag: iterKind, lo: int, hi: int): int where tag == iterKind.leader {
  yield lo;
  if hi > lo then
    on Locales((here.id+1)%numLocales) do
      for i in bar(tag, lo+1, hi) do
        yield i;
}

iter bar(param tag: iterKind, followThis, lo: int, hi: int) where tag == iterKind.follower {
  yield followThis;
}

config const n: int = 8;

forall i in bar(1, n) do
  writeln(here.id, ": ", i);
