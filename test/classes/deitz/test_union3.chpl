union foo {
  var i : integer;
  var f : float;
}

var x : foo = foo();

x.i = -1;

writeln("x: (", x.i, ")");

x.f = 2.2;

writeln("x: (", x.f, ")");

function set(out i : integer; value : integer) {
  i = value;
}

set(x.i, -1);

writeln("x: (", x.i, ")");
