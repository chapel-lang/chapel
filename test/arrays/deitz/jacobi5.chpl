fun +(x : 2*int, y : 2*int)
  return (x(1) + y(1), x(2) + y(2));

config const n = 5;
config const epsilon = 0.00001;

const stencil = (/(-1, 0), (1, 0), (0, -1), (0, 1)/);

const D = [1..n, 1..n];
const BigD = D.expand(1);
const SouthD = D.exterior(1, 0);

var A : [BigD] float;
var Temp : [BigD] float;

A(SouthD) = 1.0;
Temp(SouthD) = 1.0;

var iter = 0;

do {
  [i in D] Temp(i) = (sum reduce A(i + stencil)) / 4.0;
  iter += 1;
  if (sum reduce abs(A - Temp)) <= epsilon {
    break;
  }
  [i in D] A(i) = (sum reduce Temp(i + stencil)) / 4.0;
  iter += 1;
} while (sum reduce abs(A - Temp)) > epsilon;

writeln("Delta = ", sum reduce abs(A - Temp));
writeln("Iterations = ", iter);
