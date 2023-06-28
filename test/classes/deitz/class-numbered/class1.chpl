class foo {
  var i : int;
  var f : real;
}

var ownX = new owned foo();
var x : borrowed foo = ownX.borrow();

writeln("x: (", x.i, ", ", x.f, ")");

x.i = -1;
x.f = 3.1415;

writeln("x: (", x.i, ", ", x.f, ")");




var y : borrowed foo = x;

writeln("y: (", y.i, ", ", y.f, ")");

y.i = -2;

writeln("x: (", x.i, ", ", x.f, ")");


var ownZ = new owned foo(i=12,f=18.2);
var z : borrowed foo = ownZ.borrow();

writeln("z: (", z.i, ", ", z.f, ")");
