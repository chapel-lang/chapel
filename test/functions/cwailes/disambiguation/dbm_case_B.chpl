proc foo(a:int) {
  writeln("foo1");
}

proc foo(param a:int) {
  writeln("foo2");
}

foo(42);
