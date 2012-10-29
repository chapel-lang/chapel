use BlockDist;

config  const n: int=10;
var Dist1 = new dmap(new Block({1..n,1..n,1..n}));
var Dist2 = new dmap(new Block({1..(2*n),1..(2*n),1..(2*n)}));
var Dom1: domain(3,int) dmapped Dist1 = {1..n,1..n,1..n};
var Dom2: domain(3,int) dmapped Dist2 = {1..n,1..n,1..n};

var A:[Dom1] int(64);
var B:[Dom2] int(64) = [(i,j,k) in Dom2] i*n*n+j*n+k;

A = -1;
A[5..8,2..6,1..4]=B[2..5,5..9,3..6];
writeln(A[5..8,2..6,1..4]);
writeln();
writeln(B[2..5,5..9,3..6]);
writeln('=========');

A = -1;
A[5..5,2..6,1..4]=B[2..2,5..9,3..6];
writeln(A[5..5,2..6,1..4]);
writeln();
writeln(B[2..2,5..9,3..6]);
writeln('=========');

A = -1;
A[5..8,2..2,1..4]=B[2..5,5..5,3..6];
writeln(A[5..8,2..2,1..4]);
writeln();
writeln(B[2..5,5..5,3..6]);
writeln('=========');

A = -1;
A[5,2..6,1..4]=B[2,5..9,3..6];
writeln(A[5,2..6,1..4]);
writeln();
writeln(B[2,5..9,3..6]);
writeln('=========');

A = -1;
A[5..8,2,1..4]=B[2..5,5,3..6];
writeln();
writeln(A[5..8,2,1..4]);
writeln();
writeln(B[2..5,5,3..6]);
writeln('=========');

A = -1;
A[5..8,2..6,2]=B[2..5,5..9,4];
writeln();
writeln(A[5..8,2..6,2]);
writeln();
writeln(B[2..5,5..9,4]);
writeln('=========');

