class foo {
  var i : integer;
  var f : float;
}

var x : foo = foo();

x.i = -1;
x.f = 3.1415;

writeln("x: (", x.i, ", ", x.f, ")");

var y : foo = x;

writeln("y: (", y.i, ", ", y.f, ")");

y.i = -2;

writeln("x: (", x.i, ", ", x.f, ")");
