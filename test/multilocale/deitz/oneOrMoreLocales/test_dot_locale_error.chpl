class C {}

var c: C;

proc bar() {
  c = new C();
  return c;
}

writeln(bar().locale.id);

proc foo() return 1;

on foo() {
  writeln("hi");
}

writeln(foo().locale.id);

delete c;
