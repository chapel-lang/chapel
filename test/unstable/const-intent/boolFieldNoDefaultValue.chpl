record Foo {
  var x: bool;
}

var globalFoo = new Foo(true);

proc takesAFoo(const arg: Foo) {
  writeln(arg);
  // This should trigger the warning, but not on this line number
  globalFoo.x = false;
  writeln(arg);
}

proc main() {
  takesAFoo(globalFoo);
}
