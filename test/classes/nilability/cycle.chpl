module cycle {
  class Node { var next: borrowed Node?; }
  var a = new shared Node();
  var b = new shared Node();
  a.next = b.borrow(); // coerce from notnil to nilable
  b.next = a.borrow();

  assert(a.next == b.borrow());
  assert(b.next == a.borrow());
}
