use CyclicDist;

config const n=10;
const S = {1..n};
const R = {1..n,1..n};
const BDom: domain(1) dmapped Cyclic(startIdx=S.low)=S;
const CDom: domain(2) dmapped Cyclic(startIdx=R.low)=R;
//const BDist = new dmap(new Block(boundingBox=S));
//const RDist = new dmap(new Block(boundingBox=R));
//const BDom : domain(1) dmapped BDist=S;

//var A : [[1..n] dmapped Block(rank=1,boundingBox=[1..n])] int;
var A : [BDom] int;

for (i) in A.domain {
       A(i)=i;
}
writeln("A is:",A);

//var B : [[1..n] dmapped Block(rank=1,boundingBox=[1..n])] int; 
var B : [BDom] int;                                              
for (i) in B.domain {
       B(i)=2*i;
}
writeln("B before copy:",B);
B[1..n by 20]=A[1..n by 20];

writeln("Example 1: B after B[1..10 by 20]=A[1..10 by 30]: ",B);

for (i) in B.domain do B(i)=2*i;

B[3..8]=A[2..7];
for (a,b) in zip(B[3..8],A[2..7]) do if (a!=b) then writeln("ERROR!!!!");

writeln("Example 2: B after B[3..8]=A[2..7]: ",B);

//const CDom : domain(2) dmapped RDist=R;
var C : [CDom] int=[(i,j) in CDom] (i-1)*n + j;
var D : [CDom] int=[(i,j) in CDom] (i-1)*n + j + 499;
C[1..n by 20,1..n]=D[1..n by 20,1..n];

writeln("C: ", C);
writeln("D: ", D);
for (a,b) in zip(C[1..n by 20,1..n],D[1..n by 20,1..n]) do if (a!=b) then writeln("ERROR!!!!");