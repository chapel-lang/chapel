const n: int(64) = 10;

const D: domain(1, int(64)) = [1..n];

const A: [i in D] real(64) = i;

const B = A[1..4];

writeln("B is: ", B);

const C = A[[0..4)*2 + 1];

writeln("C is: ", C);
