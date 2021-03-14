const n = 10;

var D = {1..n, 1..n, 1..n};

writeln("D is: ", D);
writeln("D[2.., ..n-1, n/2] is: ", D[2.., ..n-1, n/2]);
writeln("D[1, 2, 3] is: ", D[1, 2, 3]);
