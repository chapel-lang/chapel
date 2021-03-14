var A: [1..3] int;
writeln(A);
A(1) = 2;
writeln(A);
var B: A.type;
writeln(B);
B(2) = 3;
writeln(B);
writeln(A);
