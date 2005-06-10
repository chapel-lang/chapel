config var n: integer = 10;

var D: domain(1) = 1..n;

var A: [D] integer = 1;

var tot: integer;

tot = sum reduce A;

writeln("result is: ", tot);
