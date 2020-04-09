var a: complex = 1.2 + 3.4i;

writeln("a is: ", a);

a.im = -a.im;

writeln("a is: ", a);


config var n = 4;

var D = {1..n};

var A: [D] complex;

[i in D] {
  A[i].re = 2*i;
  A[i].im = 2*i + 1;
}

writeln("A is: ", A);

A.im = -A.im;

writeln("A is: ", A);

