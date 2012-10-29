config var n: int = 10;

var D: domain(1) = {1..n};

var A: [D] int = 1;

var tot: int;

tot = + reduce A;

writeln("result is: ", tot);
