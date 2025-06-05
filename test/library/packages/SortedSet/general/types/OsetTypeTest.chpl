use SortedSet;



proc testSet(type t) where isTuple(t) {
  var s = new sortedSet(t, false, new defaultComparator());

  var x = (new t[0](1), new t[1](2));

  s.add(x);
  assert(s.size == 1);

  s.remove(x);
  assert(s.size == 0);

  if isUnmanagedClass(t) {
    delete x;
  }
}

proc testSet(type t) where isBorrowedClass(t) {
  var s = new sortedSet(t, false, new defaultComparator());

  // create values with 'owned' if t is a borrowed class type
  // (the set will still store borrowed)
  type useT = (t:owned);

  var x = new useT(1);

  s.add(x);
  assert(s.size == 1);

  s.remove(x.borrow());
  assert(s.size == 0);
}

proc testSet(type t) {
  var s = new sortedSet(t, false, new defaultComparator());

  var x = new t(1);

  s.add(x);
  assert(s.size == 1);

  s.remove(x);
  assert(s.size == 0);

  if isUnmanagedClass(t) {
    delete x;
  }
}
