record Foo {
  var x: int;

  proc init(x: int) {
    writeln("In user init");
    this.x = x;
  }
}
operator Foo.=(ref lhs: Foo, val: Foo) {
  writeln("In user =");
  lhs.x = val.x;
}
proc main() {
  var x: Foo = new Foo(7);
  var y: Foo = new Foo(3);
  x = y;
  writeln(x);
}
