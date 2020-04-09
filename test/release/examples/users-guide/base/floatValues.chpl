writeln("real cases:");

var myReal: real;
writeln(myReal);

myReal = 1.23;
writeln(myReal);
myReal = 1.2e3;
writeln(myReal);
myReal = 123.0e2;
writeln(myReal);
myReal = 123e2;
writeln(myReal);
myReal = 123.0E2;
writeln(myReal);
myReal = 123E2;
writeln(myReal);
myReal = 0x1p-1;
writeln(myReal);
myReal = 0x1.8;
writeln(myReal);
writeln();

writeln("imag cases:");

var myImag: imag;
writeln(myImag);

myImag = 1.23i;
writeln(myImag);
myImag = 1.2e3i;
writeln(myImag);
myImag = 123i;
writeln(myImag);
myImag = 0x0.4p-1i;
writeln(myImag);
writeln();

writeln("complex cases:");

var myComplex: complex;
writeln(myComplex);

myComplex = 1.2 + 3.4i;
writeln(myComplex);
