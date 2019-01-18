class foo {
  var i : int = 17;
  var f : real = 3.14;
}

var x : unmanaged foo = new unmanaged foo();

writeln("x: (", x.i, ", ", x.f, ")");

delete x;
