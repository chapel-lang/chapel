import Map.map;

proc testMap(type t) {

  var m = new map(int, t);

  var x: t;
  x = if isTuple(t) then (new t[0](1), new t[1](2)) else new t(1);

  var ret = m.add(1, x);
  assert(ret);

  assert(m.contains(1));
  var y: t;
  y = if isTuple(t) then (new t[0](-1), new t[1](-2)) else new t(-1);

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
