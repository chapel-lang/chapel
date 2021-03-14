var A: [1..3] int;
var t = (A, 2);
t(0)(1) = 2;
writeln(A);
