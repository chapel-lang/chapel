// Tests behavior when no zero-argument or all default value initializer
// declared for the type
record Foo {
  var x: int;

  proc init(xVal: int) {
    x = xVal;
  }
}

var f: Foo;
writeln(f);
