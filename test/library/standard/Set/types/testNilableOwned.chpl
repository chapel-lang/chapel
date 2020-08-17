import Set.set;

class C { var x: int = 0; }

proc testSet(type t) {
  var s = new set(t);

  var x = new t(1);

  // No remove call for `owned` since the set has taken ownership...
  s.add(x);
  assert(s.size == 1);
}

type T = owned C?;
testSet(T);

