proc foo(a: [] int, b: int) {
  writeln("foo1");
}

proc foo(a: int, b: int) {
  writeln("foo2");
}

var bar: [1..3] int;

foo(bar, bar);
