import List.list;

proc testList(type t) {
  /* non-nilable shared T */
  var l = new list(t);

  var x: t;
  if isTuple(t) {
    x = (new t[0](1), new t[1](2));
  } else {
    x = new t(1);
  }

  l.append(x);
  assert(l.size == 1);

  var value = l.pop();
  assert(l.size == 0);

  if isUnmanagedClass(t) {
    delete x;
  }

}
