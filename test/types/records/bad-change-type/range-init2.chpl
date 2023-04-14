proc test() {
  var ok: range(?);
  ok = 1..10 by 2;

  var err: simpleRange; /*autofix*/
  err = 1..10 by 2;
}
test();
