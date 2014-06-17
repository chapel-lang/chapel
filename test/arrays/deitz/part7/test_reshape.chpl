var D = {1..3, 1..4};
var A: [D] (int, int) = D;
writeln(A);
writeln(reshape(A, {1..4, 1..3}));

