class Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    on xVal {
      init this;
    }
  }
}

var foo = new unmanaged Foo(5);
writeln(foo);
delete foo;
