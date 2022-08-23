class Bar {
  type t;
}

proc foo(a:?T) {
  writeln("foo1");
}

proc foo(a: borrowed Bar(?t)) {
  writeln("foo2");
}


foo((new owned Bar(real)).borrow());
