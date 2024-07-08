class Foo {
  var x: imag;
  var y = 7;

  proc init(a) {
    y = a;
  }
}

var ownR = new owned Foo(3);
var r: borrowed Foo = ownR.borrow();
writeln(r);
