class Foo {
  var x: imag;
  var y = 7;

  proc init(a) {
    y = a;
  }
}

var r: borrowed Foo = new borrowed Foo(3);
writeln(r);
