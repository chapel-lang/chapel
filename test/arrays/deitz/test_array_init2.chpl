config var n : integer = 6;

var a : [i, j in 1..n, 1..n] integer = (i - 1) * n + j;

writeln(a);
