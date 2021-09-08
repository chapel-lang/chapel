class C { var x: int; }

proc main() {
  var x = new C(1);
  var v: sync borrowed C?;
  v.writeEF(x.borrow());
  var y = v.readFE();
  assert(y == x.borrow());
}
