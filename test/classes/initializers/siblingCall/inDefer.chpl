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

var foo = new Foo();
writeln(foo);
delete foo;
