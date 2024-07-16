var a1: [1..10] real;

const d = 1..2;
var a2: [d] real = 1.0;

a1[d] = a2[d];

writeln(a1);
