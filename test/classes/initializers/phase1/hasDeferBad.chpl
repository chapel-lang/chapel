class Foo {
  var x: int;

  proc init(xVal) {
    defer {
      x = xVal;
      writeln("deferred!");
    }
    writeln("should be first");
    this.complete();
    writeln("should be second");
  }
}

var foo = new unmanaged Foo(10);
writeln(foo);
delete foo;
