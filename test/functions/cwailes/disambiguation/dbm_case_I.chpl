proc foo(a:int(32)) {
  writeln("foo1");
}

proc foo(a:int(64)) {
  writeln("foo2");
}

var bar : int(32) = 42;

foo(bar);
