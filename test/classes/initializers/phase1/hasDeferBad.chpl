class Foo {
  var x: int;

  proc init(xVal) {
    defer {
      x = xVal;
      writeln("deferred!");
    }
    writeln("should be first");
    init this;
    writeln("should be second");
  }
}

var foo = new unmanaged Foo(10);
writeln(foo);
delete foo;
