import Set.set;

proc testSet(type t) {
  /* non-nilable shared T */
  var s = new set(t);

  var x = if isTuple(t) then (new t[1](1), new t[2](2))
          else new t(1);

  s.add(x);
  assert(s.size == 1);

  s.remove(x);
  assert(s.size == 0);
}
