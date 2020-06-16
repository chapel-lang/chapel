import Set.set;

proc testSet(type t) where isTuple(t) {
  var s = new set(t);

  var x = (new t[0](1), new t[1](2));

  s.add(x);
  assert(s.size == 1);

  s.remove(x);
  assert(s.size == 0);

  if isUnmanagedClass(t) {
    delete x;
  }
}

proc testSet(type t) {
  var s = new set(t);

  var x = new t(1);

  // No remove call for owned since the set has taken ownership.
  s.add(x);
  assert(s.size == 1);
}
