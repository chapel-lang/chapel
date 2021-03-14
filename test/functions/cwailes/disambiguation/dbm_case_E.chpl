proc foo(a:int) {
  writeln("foo1");
}

proc foo(a) {
  writeln("foo2");
}

foo(42);
