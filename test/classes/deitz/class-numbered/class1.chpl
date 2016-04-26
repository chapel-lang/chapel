class foo {
  var i : int;
  var f : real;
}

var x : foo = new foo();

writeln("x: (", x.i, ", ", x.f, ")");

x.i = -1;
x.f = 3.1415;

writeln("x: (", x.i, ", ", x.f, ")");




var y : foo = x;

writeln("y: (", y.i, ", ", y.f, ")");

y.i = -2;

writeln("x: (", x.i, ", ", x.f, ")");

delete x;




var z : foo = new foo(i=12,f=18.2);

writeln("z: (", z.i, ", ", z.f, ")");

delete z;
