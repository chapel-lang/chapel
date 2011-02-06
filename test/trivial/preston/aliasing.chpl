// what about aliasing of parameters?

proc zap(inout x : int, inout y : int) {
  writeln(x, ' ', y);
  x = 1;
  writeln(x, ' ', y);
  y = 2;
  writeln(x, ' ', y);
}


var x = 3, y = 4;

writeln(x, ' ', y);
zap(x, x);
writeln(x, ' ', y);

// the final value of x seems ill-defined by the spec.
