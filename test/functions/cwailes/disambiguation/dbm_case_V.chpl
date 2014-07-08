proc foo(a:int) {
  writeln("foo1");
}

proc foo(a:int) where a == 42 {
  writeln("foo2");
}

foo(42);
