use BlockDist;

var A: [{1..3} dmapped Block({1..3})] int = [i in 1..3] i;
writeln(+ scan A);
