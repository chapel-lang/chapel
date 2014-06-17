var A: [1..10] real = [i in 1..10] i;

var B => A[2..9];
writeln("B is: ", B);

var C => A[1..10 by 2];
writeln("C is: ", C);

var D => A[1..10 by -2];
writeln("D is: ", D);
