config var n = 5;

var D = {1..n, 1..n};


var D2 = D[n/2, 1..n];

var A: [D2] real = [i in D2] i;

writeln("D2 is: ", D2);

writeln("A is: ", A);
