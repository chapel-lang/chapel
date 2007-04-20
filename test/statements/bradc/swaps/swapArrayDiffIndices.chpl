config var n = 5;

var x: [i in 1..n] int = i;
var y: [i in n+1..2*n] int = -i + n;

writeln("x is: ", x);
writeln("y is: ", y);

x <=> y;

writeln("x is: ", x);
writeln("y is: ", y);
