config var n = 5;

var x: [1..n] int = [i in 1..n] i;
var y: [1..n] int = [i in 1..n] -i;

writeln("x is: ", x);
writeln("y is: ", y);

x <=> y;

writeln("x is: ", x);
writeln("y is: ", y);
