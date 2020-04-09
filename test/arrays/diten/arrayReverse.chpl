var A: [1..10] int = [i in 1..10] i;

writeln(A);
writeln("reversing even sized array");
A.reverse();
writeln(A);

var B: [1..9] int = [i in 1..9] i;
writeln(B);
writeln("reversing odd sized array");
B.reverse();
writeln(B);

var C: [11..20] int = [i in 11..20] i;
writeln(C);
writeln("reversing array with higher domain");
C.reverse();
writeln(C);
