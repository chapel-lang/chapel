const n: int(64) = 10;

const D: domain(1, int(64)) = {1..n};

const A: [D] real(64) = [i in D] i;

const B = A[1..4];

writeln("B is: ", B);

const C = A[1.. by 2 #4];

writeln("C is: ", C);
