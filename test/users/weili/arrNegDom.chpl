var A: [1..10] int = [i in 1..10] i;
writeln(A);
var B: [1..10 by -1] int = [i in 1..10 by -1] i;
writeln(B);
var C: [1..10 by -1] int = [i in 1..10] i;
writeln(C);
