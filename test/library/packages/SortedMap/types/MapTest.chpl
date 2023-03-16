import SortedMap.sortedMap;
import SortedMap.defaultComparator;

proc testMap(type t) {

  var m = new sortedMap(int, t, false, defaultComparator);

  // create values with 'owned' if t is a borrowed class type
  // (the map will still store borrowed)
  type useT = if isBorrowedClass(t) then (t:owned) else t;

  var x: useT;
  x = if isTuple(t) then (new t[0](1), new t[1](2)) else new useT(1);

  var ret = m.add(1, x);
  assert(ret);

  assert(m.contains(1));
  var y: useT;
  y = if isTuple(t) then (new t[0](-1), new t[1](-2)) else new useT(-1);

  ret = m.add(1, y);
  assert(!ret);

  ret = m.remove(1);
  assert(ret);
  assert(!m.contains(2));

  if isUnmanagedClass(t) {
    delete x;
    delete y;
  }
}
