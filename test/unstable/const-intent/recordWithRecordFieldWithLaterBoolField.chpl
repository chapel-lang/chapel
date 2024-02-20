record Bar {
  var w: int = 5;
  var x: bool;
}

record Foo {
  var x: Bar;
}

var globalFoo = new Foo(new Bar(3, true));

proc takesAFoo(const arg: Foo) {
  writeln(arg);
  // This should trigger the warning, but not on this line number
  globalFoo.x.x = false;
  writeln(arg);
}

proc main() {
  takesAFoo(globalFoo);
}
