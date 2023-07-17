proc roundIfClose(x: real) {
  const eps = 1e-8;
  if abs(x) < eps then return 0.0; // special case, to avoid -0.0
  if abs(round(x) - x) < eps then return round(x);
  return x;
}

config param n = 10;
var D = {0..n};
var A : [D] real;
var B : [D] real;
var s = (10.0 - 1.0) / n;
var m = 1.0;
A = [i in D] s * i + m;
A = roundIfClose(A);
B = acosh(A);
A = roundIfClose(A);
B = roundIfClose(B);
writeln(A);
writeln(B);
