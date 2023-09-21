// note: this test is pretty fragile based on whether or not LLVM chooses to optimize something
config var x = 10;
proc foo() {
  return 1 + x;
}
proc main() {
  var x = foo();
  writeln(x);
}
