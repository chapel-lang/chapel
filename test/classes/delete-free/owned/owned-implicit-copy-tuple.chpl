class Foo {
  var x: int;

  proc init(x: int) {
    this.x = x;
  }
}

var a = new owned Foo(1);
var b = new owned Foo(2);

var c = (a, b);

writeln(c);
