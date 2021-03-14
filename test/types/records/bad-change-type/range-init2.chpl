proc test() {
  var ok: range(?);
  ok = 1..10 by 2;

  var err: range;
  err = 1..10 by 2;
}
test();
