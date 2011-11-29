config var n = 5;

var x: [1..n] int = [i in 1..n] i;
var y: [n+1..2*n] int = [i in n+1..2*n] -i + n;

writeln("x is: ", x);
writeln("y is: ", y);

x <=> y;

writeln("x is: ", x);
writeln("y is: ", y);
