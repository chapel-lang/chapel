var A: [1..3] [1..3] [1..3] int;
writeln(A);
[(i,j,k) in {1..3, 1..3, 1..3}] A(i)(j)(k) = i*100+j*10+k;
writeln(A);
var B: [1..3] [1..3] [1..3] int;
B = A;
writeln(B);
B(1) = A(2);
writeln(B);
