proc test(type t) where isTuple(t) {
  var a = ((new t[0](1), new t[1](2)), (new t[0](3), new t[1](4)));
  assert(a.size == 2);
}

proc test(type t) where isBorrowedClass(t) {
  type useT = (t:owned);
  var elt1 = new useT(1);
  var elt2 = new useT(2);

  var a = (elt1.borrow(), elt2.borrow());
  assert(a.size == 2);
}

proc test(type t) {
  var a = (new t(1), new t(2));
  assert(a.size == 2);

  if isUnmanagedClass(t) {
    for i in a.indices {
      delete a[i];
    }
  }
}
