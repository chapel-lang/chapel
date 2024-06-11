record Bar {
  var x: bool = false;
}

record Foo {
  var x: Bar;
}

var globalFoo = new Foo(new Bar(true));

proc takesAFoo(const arg: Foo) {
  writeln(arg);
  // This should trigger the warning, but not on this line number
  globalFoo.x.x = false;
  writeln(arg);
}

proc main() {
  takesAFoo(globalFoo);
}
