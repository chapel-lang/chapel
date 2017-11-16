class Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    defer {
      writeln("deferred!");
    }
    writeln("should be first");
    super.init();
    writeln("should be second");
  }
}

var foo = new Foo(10);
writeln(foo);
delete foo;
