class C {}

var c: C;

def bar() {
  c = new C();
  return c;
}

writeln(bar().locale.id);

def foo() return 1;

on foo() {
  writeln("hi");
}

writeln(foo().locale.id);

delete c;
