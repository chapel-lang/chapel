config var n : int = 6;

var a : [{1..n, 1..n}] int = [(i,j) in {1..n, 1..n}] (i - 1) * n + j;

writeln(a);
