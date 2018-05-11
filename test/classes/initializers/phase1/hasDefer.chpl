class Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    defer {
      writeln("deferred!");
    }
    writeln("should be first");
    this.complete();
    writeln("should be second");
  }
}

var foo = new owned Foo(10);
writeln(foo);
