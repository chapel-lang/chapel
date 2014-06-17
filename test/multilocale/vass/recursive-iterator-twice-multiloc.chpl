/*
This test exposes an as-of-this-writing compiler bug by defining
two recursive iterators in the program.

A slight simplification (no 'on' clauses) is available,
if desired for debugging, as:
  functions/iterators/vass/recursive-iterator-twice-local.chpl

This test is derived from:

  functions/deitz/iterators/leader_follower/test_recursive_leader3.chpl

by duplicating+renaming all code except for 'n': bar --> bar1, bar2

This test is the same as recursive-iterator-twice-local.chpl
except it retains the 'on' clauses from test_recursive_leader3.chpl

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
   on Locales((here.id+1)%numLocales) do
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
   on Locales((here.id+1)%numLocales) do
    for i in bar2(tag, lo+1, hi) do  // invoke the leader again
      yield i;
}

iter bar2(param tag: iterKind, followThis, lo: int, hi: int) where tag == iterKind.follower {
  yield followThis;
}

forall i in bar2(1, n) do
  writeln(here.id, ": ", i);
