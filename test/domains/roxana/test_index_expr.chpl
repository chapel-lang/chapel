config var n : integer = 4;

var D : domain(2) = (1..n, 1..n);

var A : [D] float;

var iD : index(D);
var i2 : index(2);

writeln("A:\n", A);