var D = {1..5};
var A: [D] int;

ref AA = A;

AA[3] = 99;

writeln(A);
writeln(AA);
