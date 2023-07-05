use CTypes;

config const n = 10;

var A, B: [1..n] c_array(int, 3);
A = B;

writeln(A);
