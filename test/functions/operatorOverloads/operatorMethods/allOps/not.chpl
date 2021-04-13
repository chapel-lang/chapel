// Test ! operator as an operator method
record Foo {
  var x: bool;

  operator !(arg: Foo) {
    writeln("In Foo.!");
    return new Foo(!arg.x);
  }
}

proc main() {
  var a = new Foo(true);
  var b = new Foo(false);
  var c = !a;
  writeln(!a);
  writeln(!b);
  writeln(!c);
}
