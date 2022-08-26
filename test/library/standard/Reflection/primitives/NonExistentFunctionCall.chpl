
proc test() {
  param yes1 = __primitive("resolves", foo.bar.baz.ding.zoob());
  assert(!yes1);
}
test();

