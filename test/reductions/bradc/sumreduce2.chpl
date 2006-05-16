config var n: int = 10;

var D: domain(1) = [1..n];

var A: [D] float = 1.0;

var tot: float;

tot = sum(float) reduce A;

writeln("result is: ", tot);
