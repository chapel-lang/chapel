use BlockDist;

config const n=10;
const S = {1..n};
const BDist = new dmap(new Block(boundingBox=S));
const BDom : domain(1) dmapped BDist=S;

//var A : [[1..n] dmapped Block(rank=1,boundingBox=[1..n])] int;
var A : [BDom] int;
var B : [BDom] int;
writeln("A and B defined on ",S);
for (i) in A.domain {
       A(i)=i;
       B(i)=2*i;
}

//writeln("A is:",A);
var Dom1 = {1..n by 4};
var Dom2 = {1..n by 4};

writeln("Example 1: B",Dom2,"(",Dom2.size,") = A",Dom1,"(",Dom1.size,")");
B[Dom2]=A[Dom1];

for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}

Dom1 = {1..n/2 by 2};
Dom2 = {1..n by 4};
writeln("Example 2: B",Dom2,"(",Dom2.size,") = A",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom1 = {1..n/4 by 2};
Dom2 = {1..n/2 by 4};
writeln("Example 3: B",Dom2,"(",Dom2.size,") = A",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
//if ((n/4)%2 == (n/2)%4)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom1 = {1..n by n};
Dom2 = {1..n by 2*n};
writeln("Example 4: B",Dom2,"(",Dom2.size,") = A",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom1 = {1..n/2 by 3};
Dom2 = {1..n/3 by 2};
writeln("Example 5: B",Dom2,"(",Dom2.size,") = A",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
//if ((n/2)%3 == (n/3)%2)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom1 = {2..n by n};
Dom2 = {2..n by 2*n};
writeln("Example 6: B",Dom2,"(",Dom2.size,") = A",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom1 = {2..n by 4};
Dom2 = {2..n by 4};
writeln("Example 7: B",Dom2,"(",Dom2.size,") = A",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom1 = {2..n/2 by 2};
Dom2 = {2..n by 4};
writeln("Example 8: B",Dom2,"(",Dom2.size,") = A",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
//if ((n-1)%4 == ((n-1)/2)%2)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom1 = {2..n/2 by 2};
Dom2 = {4..n by 4};
writeln("Example 9: B: ",Dom2,"(",Dom2.size,") = A: ",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom1 = {2..n/2-1 by 2};
Dom2 = {4..n-1 by 4};
writeln("Example 10: B: ",Dom2,"(",Dom2.size,") = A: ",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom1 = {1..n/2-1 by 2};
Dom2 = {1..n-1 by 4};
writeln("Example 11: B: ",Dom2,"(",Dom2.size,") = A: ",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom1 = {(n/2)+1..n by 2};
Dom2 = {1..n by 4};
writeln("Example 12: B: ",Dom2,"(",Dom2.size,") = A: ",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom1 = {(n/2)+1..3*n/4 by 2};
Dom2 = {1..n/2 by 4};
writeln("Example 13: B: ",Dom2,"(",Dom2.size,") = A: ",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}


Dom1 = {(n/2)+1..3*n/4 by 2};
Dom2 = {1..n/2 by 4};
writeln("Example 14: B: ",Dom2,"(",Dom2.size,") = A: ",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}


Dom1 = {(n/2)+1..3*n/4 by 2};
Dom2 = {(n/4)+1..3*n/4 by 4};
writeln("Example 15: B: ",Dom2,"(",Dom2.size,") = A: ",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom1 = {(n/2)+1..3*n/4 by 2};
Dom2 = {(n/2)+1..n by 4};
writeln("Example 16: B: ",Dom2,"(",Dom2.size,") = A: ",Dom1,"(",Dom1.size,")");
if (Dom2.size == Dom1.size)
{
	B[Dom2]=A[Dom1];
	for (a,b) in zip(A[Dom1],B[Dom2]) do if (a!=b) then {writeln("ERROR!!!!");}
}

const R = {n/2+1..3*n/2};
const BRDist = new dmap(new Block(boundingBox=R));
const BRDom : domain(1) dmapped BRDist=R;

var C : [BRDom] int;
var D : [BRDom] int;
writeln(" C and D defined on ",R);

for (i) in C.domain {
       C(i)=i;
       D(i)=2*i;
}

var Dom3 = {(n/2)+1..3*n/4 by 2};
var Dom4 = {(n/2)+1..n by 4};

 writeln("Example 17: D: ",Dom4,"(",Dom4.size,") = C: ",Dom3,"(",Dom3.size,")");
if (Dom4.size == Dom3.size)
{
	D[Dom4]=C[Dom3];
	for (a,b) in zip(C[Dom3],D[Dom4]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom3 = {3*n/4..n by 2};
Dom4 = {(n/2)+1..n by 4};
writeln("Example 18: D: ",Dom4,"(",Dom4.size,") = C: ",Dom3,"(",Dom3.size,")");
if (Dom4.size == Dom3.size)
{
	D[Dom4]=C[Dom3];
	for (a,b) in zip(C[Dom3],D[Dom4]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom3 = {3*n/4..5*n/4 by 2};
Dom4 = {(n/2)+1..3*n/2 by 4};
writeln("Example 19: D: ",Dom4,"(",Dom4.size,") = C: ",Dom3,"(",Dom3.size,")");
if (Dom4.size == Dom3.size)
{
	D[Dom4]=C[Dom3];
	for (a,b) in zip(C[Dom3],D[Dom4]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom3 = {n+1..3*n/2 by 2};
Dom4 = {(n/2)+1..3*n/2 by 4};
writeln("Example 20: D: ",Dom4,"(",Dom4.size,") = C: ",Dom3,"(",Dom3.size,")");
if (Dom4.size == Dom3.size)
{
	D[Dom4]=C[Dom3];
	for (a,b) in zip(C[Dom3],D[Dom4]) do if (a!=b) then {writeln("ERROR!!!!");}
}

const T = {n/2+1..5*n/2};
const BTDist = new dmap(new Block(boundingBox=T));
const BTDom : domain(1) dmapped BTDist=T;

var E : [BTDom] int;
writeln("E defined on ",T);


var Dom5 = {n+1..3*n/2 by 2};
Dom1 = {1..n by 4};
writeln("Example 21: A: ",Dom1,"(",Dom1.size,") = E: ",Dom5,"(",Dom5.size,")");
if (Dom1.size == Dom5.size)
{
	A[Dom1]=E[Dom5];
	for (a,b) in zip(A[Dom1],E[Dom5]) do if (a!=b) then {writeln("ERROR!!!!");}
}

writeln("Example 22: E: ",Dom5,"(",Dom5.size,") = A: ",Dom1,"(",Dom1.size,")");
if (Dom1.size == Dom5.size)
{
	E[Dom5]=A[Dom1];
	for (a,b) in zip(A[Dom1],E[Dom5]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom5 = {(4*n/2)+1..5*n/2 by 2};
Dom1 = {1..n by 4};
writeln("Example 23: A: ",Dom1,"(",Dom1.size,") = E: ",Dom5,"(",Dom5.size,")");
if (Dom1.size == Dom5.size)
{
	A[Dom1]=E[Dom5];
	for (a,b) in zip(A[Dom1],E[Dom5]) do if (a!=b) then {writeln("ERROR!!!!");}
}


Dom5 = {(8*n/4)+1..9*n/4 by 2};
Dom1 = {(n/2)+1..n by 4};
writeln("Example 24: A: ",Dom1,"(",Dom1.size,") = E: ",Dom5,"(",Dom5.size,")");
if (Dom1.size == Dom5.size)
{
	A[Dom1]=E[Dom5];
	for (a,b) in zip(A[Dom1],E[Dom5]) do if (a!=b) then {writeln("ERROR!!!!");}
}

Dom5 = {(3*n/2)+1..7*n/4 by 2};
Dom1 = {(n/4)+1..3*n/4 by 4};
writeln("Example 25: A: ",Dom1,"(",Dom1.size,") = E: ",Dom5,"(",Dom5.size,")");
if (Dom1.size == Dom5.size)
{
	A[Dom1]=E[Dom5];
	for (a,b) in zip(A[Dom1],E[Dom5]) do if (a!=b) then {writeln("ERROR!!!!");}
}


Dom5 = {(3*n/2)+1..5*n/2 by 4};
Dom1 = {(n/4)+1..3*n/4 by 2};
writeln("Example 26: A: ",Dom1,"(",Dom1.size,") = E: ",Dom5,"(",Dom5.size,")");
if (Dom1.size == Dom5.size)
{
	A[Dom1]=E[Dom5];
	for (a,b) in zip(A[Dom1],E[Dom5]) do if (a!=b) then {writeln("ERROR!!!!");}
}