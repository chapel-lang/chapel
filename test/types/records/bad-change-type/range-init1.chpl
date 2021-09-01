proc test() {
  var ok: range(?) = 1..10 by 2;

  var err: range(stridable=false) = 1..10 by 2;
}
test();
