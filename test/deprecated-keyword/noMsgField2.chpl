record Foo {
  deprecated var x: int;
}

proc main() {
  var foo = new Foo();
  writeln(foo.x);
}
