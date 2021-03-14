type t = real;

proc foo(A: [?ADom] t) {
  const BigADom = ADom.expand(1);
  var Temp: [BigADom] t = 1.0;

  Temp[ADom] = A;

  writeln("Temp is:\n", Temp);
}

config const n = 4;
const D = {1..n, 1..n};
var A: [D] real;

[(i,j) in D] A(i,j) = i + j/10.0;

writeln("A is:\n", A);

foo(A);
