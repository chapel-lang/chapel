class Bar {
  type t;
}

proc foo(a: Bar(?t)) {
  writeln("foo1");

  delete a;
}

proc foo(a:?T) {
  writeln("foo2");

  delete a;
}

foo(new Bar(real));
