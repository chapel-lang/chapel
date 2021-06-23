record mycomplex {
  var re : real;
  var im : real;
}

operator mycomplex.+(x : mycomplex, y : mycomplex)
  return new mycomplex(x.re + y.re, x.im + y.im);

operator +(x : mycomplex, y : real)
  return new mycomplex(x.re + y, x.im);

operator +(x : real, y : mycomplex)
  return new mycomplex(x + y.re, y.im);

operator mycomplex.-(x : mycomplex, y : mycomplex)
  return new mycomplex(x.re - y.re, x.im - y.im);

operator -(x : mycomplex, y : real)
  return new mycomplex(x.re - y, x.im);

operator -(x : real, y : mycomplex)
  return new mycomplex(x - y.re, -y.im);

operator mycomplex.*(x : mycomplex, y : mycomplex)
  return new mycomplex(x.re*y.re - x.im*y.im, x.im*y.re + x.re*y.im);

operator *(x : mycomplex, y : real)
  return new mycomplex(x.re*y, x.im*y);

operator *(x : real, y : mycomplex)
  return new mycomplex(x*y.re, x*y.im);

operator mycomplex./(x : mycomplex, y : mycomplex)
  return let d = y.re*y.re + y.im*y.im in
    new mycomplex((x.re*y.re + x.im*y.im)/d, (x.im*y.re - x.re*y.im)/d);

operator /(x : mycomplex, y : real)
  return new mycomplex(x.re/y, x.im/y);

operator /(x : real, y : mycomplex)
  return let d = y.re*y.re + y.im*y.im in
    new mycomplex(x*y.re/d, -x*y.im/d);

var x : mycomplex = new mycomplex(re = 8.0, im = 1.0);
var y : mycomplex = new mycomplex(re = 4.0, im = 2.0);

writeln("x = ", x);
writeln("y = ", y);
writeln();
writeln("x + y   = ", x + y);
writeln("x + 4.0 = ", x + 4.0);
writeln("2.0 + y = ", 2.0 + y);
writeln();
writeln("x - y   = ", x - y);
writeln("x - 4.0 = ", x - 4.0);
writeln("2.0 - y = ", 2.0 - y);
writeln();
writeln("x * y   = ", x * y);
writeln("x * 4.0 = ", x * 4.0);
writeln("2.0 * y = ", 2.0 * y);
writeln();
writeln("x / y   = ", x / y);
writeln("x / 4.0 = ", x / 4.0);
writeln("2.0 / y = ", 2.0 / y);
