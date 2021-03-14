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

  s.add(x);
  assert(s.size == 1);

  s.remove(x);
  assert(s.size == 0);

  if isUnmanagedClass(t) {
    delete x;
  }
}
