config var n = 5;

var D1 = {1..n};
var D2 = {1..2*n};

var x: [D1] int = [i in D1] i;
var y: [D2] int = [i in D2] -i;

writeln("x is: ", x);
writeln("y is: ", y);

D1 <=> D2;

writeln("x is: ", x);
writeln("y is: ", y);

D1 = {1..n+1};

writeln("x is: ", x);
writeln("y is: ", y);
