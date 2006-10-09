config var n = 4;

const D = [1..n];

var A: [D] complex;

[i in D] A(i) = i:float*2;
writeln("A is: ", A);

[i in D] A(i) = (i:float)*3;
writeln("A is: ", A);

[i in D] A(i) = 4*i:float;
writeln("A is: ", A);


