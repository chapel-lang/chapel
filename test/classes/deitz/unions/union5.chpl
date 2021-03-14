union foo {
  var i : int;
  var f : real;
}

class C {
  var c;
}

var x = new unmanaged C(new foo());

x.c.i = -1;

writeln("x: (", x.c.i, ")");

x.c.f = 2.2;

writeln("x: (", x.c.f, ")");

delete x;
