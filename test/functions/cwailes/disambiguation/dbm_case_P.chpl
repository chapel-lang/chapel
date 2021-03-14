proc foo(x: [] int) {
  writeln("foo1");
}

proc foo(x: int) {
  writeln("foo2");
}

var bar: [1..3] int;

foo(bar);
