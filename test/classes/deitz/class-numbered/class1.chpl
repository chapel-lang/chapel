class foo {
  var i : int;
  var f : real;
}

var x : borrowed foo = (new owned foo()).borrow();

writeln("x: (", x.i, ", ", x.f, ")");

x.i = -1;
x.f = 3.1415;

writeln("x: (", x.i, ", ", x.f, ")");




var y : borrowed foo = x;

writeln("y: (", y.i, ", ", y.f, ")");

y.i = -2;

writeln("x: (", x.i, ", ", x.f, ")");


var z : borrowed foo = (new owned foo(i=12,f=18.2)).borrow();

writeln("z: (", z.i, ", ", z.f, ")");
