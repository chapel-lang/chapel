use Functions;

var f:func(int, int) = lambda(i:int) { return i+1; };

writeln(f(10));

var g = lambda(j:int) { return j+2; };
f = g;

writeln(f(10));

