use CTypes;

extern record Foo {
  var value: c_int;
}

proc test() {
  var foo: Foo;
  writeln(foo);
}
test();

