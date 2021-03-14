var A: [1..3] real = [1.0, 2.0, 3.0];

var z = + reduce ([i in 1..0] (10 * A));
writeln(z);
