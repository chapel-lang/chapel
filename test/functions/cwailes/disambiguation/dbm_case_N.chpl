proc foo(a:uint(64)) {
  writeln("foo1");
}

proc foo(a:int(8)) {
  writeln("foo2");
}

foo(42:int(64));
