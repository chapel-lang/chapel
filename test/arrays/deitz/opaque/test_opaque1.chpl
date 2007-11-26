var D: opaque domain;
var i1 = D.new();
var i2 = D.new();
var A: [D] int;
A(i1) = 4;
A(i2) = 5;
writeln(A(i1));
writeln(A(i2));
