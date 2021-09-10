class C { var x: int; }

proc main() {
  var x = new unmanaged C?(1);
  var v: sync unmanaged C?;
  v.writeEF(x);
  var y = v.readFE();
  assert(y == x);
  delete x;
}
