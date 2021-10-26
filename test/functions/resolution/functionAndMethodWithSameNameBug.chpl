proc foo(arg1, arg2) {
  writeln("Module foo called");
}
record R {
  proc foo(arg1) {
    writeln("Record foo called");
    foo(arg1, 10);
  }
}
var r: R;
r.foo(5);

