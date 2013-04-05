/*
This test exposes an as-of-this-writing compiler bug by defining
two recursive iterators in the program.

This test is derived from:

  functions/deitz/iterators/leader_follower/test_recursive_leader3.chpl

with these modifications:

* the code (except for 'n') is duplicated: bar --> bar1, bar2
* the 'on' clause is removed from the leader,
  since the bug is still triggered

Note that if we simplify it by using only the serial iterator for bar1:

  iter bar1(lo: int, hi: int):int {
    yield lo;
    if hi > lo then
      for i in bar1(lo+1, hi) do
        yield i;
  }
  for i in bar1(1, n) do
    writeln(here.id, ": ", i);

then the bug is not triggered, even with --no-local.
*/

config const n: int = 8;

/////////////////////////////////

iter bar1(lo: int, hi: int) {
  for i in lo..hi do
    yield i;
}

iter bar1(param tag: iterKind, lo: int, hi: int): int where tag == iterKind.leader {
  yield lo;
  if hi > lo then
    for i in bar1(tag, lo+1, hi) do  // invoke the leader again
      yield i;
}

iter bar1(param tag: iterKind, followThis, lo: int, hi: int) where tag == iterKind.follower {
  yield followThis;
}

forall i in bar1(1, n) do
  writeln(here.id, ": ", i);

/////////////////////////////////

iter bar2(lo: int, hi: int) {
  for i in lo..hi do
    yield i;
}

iter bar2(param tag: iterKind, lo: int, hi: int): int where tag == iterKind.leader {
  yield lo;
  if hi > lo then
    for i in bar2(tag, lo+1, hi) do  // invoke the leader again
      yield i;
}

iter bar2(param tag: iterKind, followThis, lo: int, hi: int) where tag == iterKind.follower {
  yield followThis;
}

forall i in bar2(1, n) do
  writeln(here.id, ": ", i);
