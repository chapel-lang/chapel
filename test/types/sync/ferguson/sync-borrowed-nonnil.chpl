class C { var x: int; }

proc main() {
  var x = new C(1);
  var v: sync borrowed C = x.borrow();
  var a = v.readFF();
  assert(a == x.borrow());
  var y = v.readFE();
  assert(y == x.borrow());
}
