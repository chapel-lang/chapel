var A = [1, 2, 3, 4, 5];

writeln(A.find(3));
writeln(A.find(2));
writeln(A.find(0));

var B: [1..3, 1..3] int;
forall (i,j) in {1..3, 1..3} do
  B[i,j] = (i-1)*3 + j;

writeln(B.find(5));
writeln(B.find(3));
writeln(B.find(4));
writeln(B.find(10));
