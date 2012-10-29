use BlockDist;

config var n: int = 8;

var A: [{1..n} dmapped Block({1..n})] int;

writeln(for i in 1..n do (i, A(i), A(i).locale.id));
