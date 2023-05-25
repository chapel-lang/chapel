var A: [1..10] int = [i in 1..10] i;
writeln(A);
var B: [1..10 by -1] int = [i in 1..10 by -1] i;   // neg-stride
writeln(B);
var C: [1..10 by -1] int = [i in 1..10] i;         // neg-stride
writeln(C);
