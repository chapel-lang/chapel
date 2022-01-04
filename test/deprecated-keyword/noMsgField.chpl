record Foo {
  deprecated var x: int;

  proc init() {  }
}

proc main() {
  var foo = new Foo();
  writeln(foo.x);
}
