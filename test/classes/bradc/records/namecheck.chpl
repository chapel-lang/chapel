class R {
  var x: int;
  var y: real;
}

var x: int = 3;
var y: real = 1.2;

var r = new unmanaged R(x = x, y = y);

writeln("r is: ", r);

delete r;
