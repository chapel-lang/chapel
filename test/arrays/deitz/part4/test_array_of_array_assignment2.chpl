var A: [1..3] [1..3] int;
[(i,j) in {1..3, 1..3}] A(i)(j) = 3*(i-1)+j;
writeln(A);
var B: [1..3] [1..3] int;
B(2) = A(3);
writeln(B);

