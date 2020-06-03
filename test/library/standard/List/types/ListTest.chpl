import List.list;

proc testList(type t) where isTuple(t) {
  var l = new list(t);
  var x = (new t[0](1), new t[1](2));

  l.append(x);
  assert(l.size == 1);

  var value = l.pop();
  assert(l.size == 0);
}

proc testList(type t) {
  var l = new list(t);

  var x: t = new t(1);

  l.append(x);
  assert(l.size == 1);

  var value = l.pop();
  assert(l.size == 0);

  if isUnmanagedClass(t) {
    delete x;
  }

}
