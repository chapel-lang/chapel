config var n = 10;

const D = {1..n, 1..n};

writeln("D is: ", D);

var DSlice = D[1..n by 2, 2..n by 2];

writeln("DSlice is: ", DSlice);

DSlice = D[1..n-1, 2..n];

writeln("DSlice is: ", DSlice);

DSlice = D[0..n-1, 2..n+1];

writeln("DSlice is: ", DSlice);
