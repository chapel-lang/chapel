class Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    defer {
      writeln("deferred!");
    }
    writeln("should be first");
    init this;
    writeln("should be second");
  }
}

var foo = new owned Foo(10);
writeln(foo);
