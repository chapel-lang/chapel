record mycomplex {
  var re : float;
  var im : float;
}

def +(x : mycomplex, y : mycomplex)
  return mycomplex(x.re + y.re, x.im + y.im);

def +(x : mycomplex, y : float)
  return mycomplex(x.re + y, x.im);

def +(x : float, y : mycomplex)
  return mycomplex(x + y.re, y.im);

def -(x : mycomplex, y : mycomplex)
  return mycomplex(x.re - y.re, x.im - y.im);

def -(x : mycomplex, y : float)
  return mycomplex(x.re - y, x.im);

def -(x : float, y : mycomplex)
  return mycomplex(x - y.re, -y.im);

def *(x : mycomplex, y : mycomplex)
  return mycomplex(x.re*y.re - x.im*y.im, x.im*y.re + x.re*y.im);

def *(x : mycomplex, y : float)
  return mycomplex(x.re*y, x.im*y);

def *(x : float, y : mycomplex)
  return mycomplex(x*y.re, x*y.im);

def /(x : mycomplex, y : mycomplex)
  return let d = y.re*y.re + y.im*y.im in
    mycomplex((x.re*y.re + x.im*y.im)/d, (x.im*y.re - x.re*y.im)/d);

def /(x : mycomplex, y : float)
  return mycomplex(x.re/y, x.im/y);

def /(x : float, y : mycomplex)
  return let d = y.re*y.re + y.im*y.im in
    mycomplex(x*y.re/d, -x*y.im/d);

var x : mycomplex = mycomplex(re = 8.0, im = 1.0);
var y : mycomplex = mycomplex(re = 4.0, im = 2.0);

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
