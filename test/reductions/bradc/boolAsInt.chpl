config var n = 10;

var D = {1..n};

var A: [D] int = [i in D] i;

var numEvens = + reduce [i in D] (A(i)%2 == 0);

writeln("numEvens = ", numEvens);

