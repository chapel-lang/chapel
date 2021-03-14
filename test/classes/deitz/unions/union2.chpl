union foo {
  var i : int;
  var f : real;
}

var x : foo = new foo();

x.i = -1;

writeln("x: (", x.i, ")");

x.f = 2.2;

writeln("x: (", x.i, ")");

