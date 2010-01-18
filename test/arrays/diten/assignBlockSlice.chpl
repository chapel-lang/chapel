use BlockDist;

var Dist = new dist(new Block(rank=2, bbox=[1..10, 1..10]));
var dom: domain(2) distributed Dist = [1..10, 1..10];
var A: [dom] int;

A[2..10 by 2, 1..10 by 3] = 1;
A[3..6, 3..6] = 2;
var B = A[1..10, 1..10 by 2];
writeln(A);
writeln(B);
writeln(B.domain);
B[1..5, 1..10 by 2] = 3;
writeln(A);
