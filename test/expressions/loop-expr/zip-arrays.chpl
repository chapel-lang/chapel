
var A, B : [1..4] int;
var C = [ab in zip(A,B)] ab;  // C: [1..4] (2*int)
writeln(C);
