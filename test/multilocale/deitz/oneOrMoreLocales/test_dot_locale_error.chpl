class C {}

def bar() return new C();

writeln(bar().locale.id);

def foo() return 1;

on foo() {
  writeln("hi");
}

writeln(foo().locale.id);
