class C {}

proc foo(const ref arg: (shared C?,)) {
  writeln(arg);
}

proc main() {
  var t1 = (new shared C(), );
  foo(t1);
}
