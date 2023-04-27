// test from https://github.com/chapel-lang/chapel/issues/5544
proc times2(f) {
  return 2*f();
}
proc test(k : int) {
  var func = lambda() { return k;};
  writeln(times2(func));
}
test(2);
