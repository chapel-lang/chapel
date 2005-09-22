-- what about aliasing of parameters?

function zap(inout x : integer, inout y : integer) {
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

-- the final value of x seems ill-defined by the spec.
