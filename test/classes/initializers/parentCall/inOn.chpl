class Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    on xVal {
      super.init();
    }
  }
}

var foo = new Foo(5);
writeln(foo);
delete foo;
