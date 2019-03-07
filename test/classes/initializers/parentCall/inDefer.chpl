class Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    defer {
      super.init();
    }
  }
}

var foo = new unmanaged Foo(3);
writeln(foo);
delete foo;
