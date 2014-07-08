class Bar {
  type t;
}

proc foo(a:?T) {
  writeln("foo1");
}

proc foo(a: Bar(?t)) {
  writeln("foo2");
}

foo(new Bar(real));
