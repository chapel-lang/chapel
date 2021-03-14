const vecDom = {1..3};
type vec = [vecDom] real;
var A: vec;

[i in vec] A(i) = i;

writeln("A is: ", A);
