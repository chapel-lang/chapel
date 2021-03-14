var f = lambda(x:int) { return x + 1; };
var g = lambda(y:int) { return y + 2; };

writeln(f(10));
writeln(g(10));
f = g;
writeln(f(10));
