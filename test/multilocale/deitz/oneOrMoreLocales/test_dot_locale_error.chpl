class C {}

var c: unmanaged C?;

proc bar() {
  c = new unmanaged C();
  return c;
}

writeln(bar().locale.id);

proc foo() return 1;

on foo() {
  writeln("hi");
}

writeln(foo().locale.id);

delete c;
