import Map.map;

proc testMap(type t) {
  /* non-nilable shared T */
  var m = new map(int, t);

  var x: t;
  if isTuple(t) {
    x = (new t[0](1), new t[1](2));
  } else {
    x = new t(1);
  }

  var ret = m.add(1, x);
  assert(ret);
  assert(m.contains(1));

  var y: t;
  if isTuple(t) {
    y = (new t[0](-1), new t[1](-2));
  } else {
    y = new t(-1);
  }
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
