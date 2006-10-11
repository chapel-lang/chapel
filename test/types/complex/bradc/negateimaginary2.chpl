var a: complex = 1.2 + 3.4i;

writeln("a is: ", a);

a.imag = -a.imag;

writeln("a is: ", a);


config var n = 4;

var D = [1..n];

var A: [D] complex;

[i in D] {
  A[i].real = 2*i;
  A[i].imag = 2*i + 1;
}

writeln("A is: ", A);

A.imag = -A.imag;

writeln("A is: ", A);

