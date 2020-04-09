use CyclicDist;

config const printOutput=false;
config  const n: int=10;
const S = {1..n,1..n,1..n};
const Dom1: domain(3) dmapped Cyclic(startIdx=S.low)=S;
const S2 = {1..2*n,1..2*n,1..2*n};
const Dom2: domain(3) dmapped Cyclic(startIdx=S2.low)=S2;
/*var Dist1 = new dmap(new Block({1..n,1..n,1..n}));
var Dist2 = new dmap(new Block({1..(2*n),1..(2*n),1..(2*n)}));
var Dom1: domain(3,int) dmapped Dist1 ={1..n,1..n,1..n};
var Dom2: domain(3,int) dmapped Dist2 ={1..n,1..n,1..n};
*/
var A:[Dom1] int(64);
var B:[Dom2] int(64) = [(i,j,k) in Dom2] i*n*n+j*n+k;

var Dom3={5..8,2..6,1..4};
var Dom4={2..5,5..9,3..6};

A = -1;
A[Dom3]=B[Dom4];
writeln(A[Dom3]);
writeln();
writeln(B[Dom4]);
if printOutput then writeln("Example 1");
for (a,b) in zip(A[Dom3],B[Dom4]) do if (a!=b) then {writeln("ERROR!!!!");}
writeln('=========');

A = -1;
Dom3={5..5,2..6,1..4};
Dom4={2..2,5..9,3..6};
A[Dom3]=B[Dom4];
writeln(A[Dom3]);
writeln();
writeln(B[Dom4]);
if printOutput then writeln("Example 2");
for (a,b) in zip(A[Dom3],B[Dom4]) do if (a!=b) then {writeln("ERROR!!!!");}
writeln('=========');

A = -1;
Dom3={5..8,2..2,1..4};
Dom4={2..5,5..5,3..6};
A[Dom3]=B[Dom4];
writeln(A[Dom3]);
writeln();
writeln(B[Dom4]);
if printOutput then writeln("Example 3");
for (a,b) in zip(A[Dom3],B[Dom4]) do if (a!=b) then {writeln("ERROR!!!!");}
writeln('=========');

A = -1;
A[5,2..6,1..4]=B[2,5..9,3..6];
writeln(A[5,2..6,1..4]);
writeln();
writeln(B[2,5..9,3..6]);
for (a,b) in zip(A[5,2..6,1..4],B[2,5..9,3..6]) do if (a!=b) then {writeln("ERROR!!!!");}
writeln('=========');

A = -1;
if printOutput then writeln("Example 4");
A[5..8,2,1..4]=B[2..5,5,3..6];
writeln();
writeln(A[5..8,2,1..4]);
writeln();
writeln(B[2..5,5,3..6]);
for (a,b) in zip(A[5..8,2,1..4],B[2..5,5,3..6]) do if (a!=b) then {writeln("ERROR!!!!");}
writeln('=========');

A = -1;
if printOutput then writeln("Example 5");
A[5..8,2..6,2]=B[2..5,5..9,4];
writeln();
writeln(A[5..8,2..6,2]);
writeln();
writeln(B[2..5,5..9,4]);
for (a,b) in zip(A[5..8,2..6,2],B[2..5,5..9,4]) do if (a!=b) then {writeln("ERROR!!!!");}
writeln('=========');