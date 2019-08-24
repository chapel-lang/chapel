var D: domain(opaque);
var i1 = D.create();
var i2 = D.create();
var A: [D] int;
A(i1) = 4;
A(i2) = 5;
writeln(A(i1));
writeln(A(i2));
