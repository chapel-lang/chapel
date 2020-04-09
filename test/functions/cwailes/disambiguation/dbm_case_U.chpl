proc foo(param a:int) where a == 42 {
  writeln("foo1");
}

proc foo(param a:int) {
  writeln("foo2");
}

foo(42);
