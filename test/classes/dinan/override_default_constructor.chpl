class Foo {
  var x: int;

  proc init(y: int) {
    x = 2*y;
  }
}

var bar = new unmanaged Foo(5);

writeln(bar.x);

delete bar;
