import SortedMap.sortedMap;
import SortedMap.DefaultComparator;

proc testMap(type t) where isTupleType(t) {
  var m = new sortedMap(int, t, false, new DefaultComparator());

  var x: t = (new t[0](1), new t[1](2));

  var ret = m.add(1, x);
  assert(ret);

  assert(m.contains(1));
  var y: t = (new t[0](-1), new t[1](-2));

  ret = m.add(1, y);
  assert(!ret);

  ret = m.remove(1);
  assert(ret);
  assert(!m.contains(1));
  assert(!m.contains(2));
}

proc testMap(type t) where isBorrowedClass(t) {
  var m = new sortedMap(int, t, false, new DefaultComparator());

  // create values with 'owned' if t is a borrowed class type
  // (the map will still store borrowed)
  type useT = (t:owned);

  var x: useT = new useT(1);;

  var ret = m.add(1, x.borrow());
  assert(ret);

  assert(m.contains(1));
  var y: useT = new useT(-1);

  ret = m.add(1, y.borrow());
  assert(!ret);

  ret = m.remove(1);
  assert(ret);
  assert(!m.contains(1));
  assert(!m.contains(2));
}

proc testMap(type t) {
  var m = new sortedMap(int, t, false, new DefaultComparator());

  var x: t = new t(1);

  var ret = m.add(1, x);
  assert(ret);

  assert(m.contains(1));
  var y: t = new t(-1);

  ret = m.add(1, y);
  assert(!ret);

  ret = m.remove(1);
  assert(ret);
  assert(!m.contains(1));
  assert(!m.contains(2));

  if isUnmanagedClass(t) {
    delete x;
    delete y;
  }
}
