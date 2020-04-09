iter itr() {
  for i in 1..5 do
    yield i;
}

proc itr(param tag: iterKind) where tag == iterKind.leader { }
proc itr(param tag: iterKind, followThis) where tag == iterKind.follower { }

forall i in itr() do
  writeln(i);
