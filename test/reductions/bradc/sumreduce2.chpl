config var n: int = 10;

var D: domain(1) = {1..n};

var A: [D] real = 1.0;

var tot: real;

tot = + reduce A;

writeln("result is: ", tot);
