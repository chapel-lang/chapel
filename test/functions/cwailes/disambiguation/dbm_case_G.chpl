class Bar {
  type t;
}

proc foo(a: borrowed Bar(?t)) {
  writeln("foo1");
}

proc foo(a:?T) {
  writeln("foo2");
}

foo(new borrowed Bar(real));
