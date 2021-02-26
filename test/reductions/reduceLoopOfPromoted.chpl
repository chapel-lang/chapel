var A: [1..3] real = [1.0, 2.0, 3.0];

var x = + reduce (10 * A);
var y = + reduce ([i in 1..3] A);
var z = + reduce ([i in 1..3] (10 * A));
writeln(x);
writeln(y);
writeln(z);
