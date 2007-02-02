config const n = 10;

const D = [1..n];

var A: [i in D] real = i-(n/2),
    B: [i in D] real = n;

var C: [D] bool;

writeln("A is: ", A);
writeln("B is: ", B);

//C = (A != 0) && (B/A > 0);

writeln("C is: ", C);
