class Foo {
  var x: int;

  proc init(xVal) {
    on xVal {
      this.init(xVal, 2);
    }
  }

  proc init(a, b) {
    x = a + b;
  }
}

var foo = new Foo(5);
writeln(foo);
delete foo;
