class Foo {
  var x: int;

  proc init() {
    defer {
      this.init(3);
    }
  }

  proc init(xVal) {
    x = xVal;
  }
}

var foo = new unmanaged Foo();
writeln(foo);
delete foo;
