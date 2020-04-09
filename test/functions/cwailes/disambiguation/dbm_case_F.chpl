proc foo(a) {
  writeln("foo1");
}

proc foo(a:int) {
  writeln("foo2");
}

foo(42);
