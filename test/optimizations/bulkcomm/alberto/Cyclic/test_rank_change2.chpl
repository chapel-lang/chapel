config  const n: int=5;
var A: [1..5, 1..5] int = [(i,j) in {1..5, 1..5}] (i-1)*5 + j;
var F: [1..5, 1..5] int = [(i,j) in {1..5, 1..5}] (i-1)*5 + j + 299;

var B: [1..5] int;

var C: [1..5,1..5,1..5] int
     = [(i,j,k) in {1..5, 1..5, 1..5}] (i-1)*5*5 + (j-1)*5 + k + 99;

var E: [1..5,1..5,1..5] int
     = [(i,j,k) in {1..5, 1..5, 1..5}] (i-1)*5*5 + (j-1)*5 + k + 499;

var D: [1..5,1..5,1..5,1..5] int
     = [(i,j,k,l) in {1..5, 1..5, 1..5, 1..5}]
         (i-1)*5*5*5 + (j-1)*5*5 + (k-1)*5 + l + 9999;;
//writeln("A:",A);
//writeln("B:",B);
//writeln("C:",C);
//writeln("D: ",D);

writeln("Example 1");
F[1..5,1..5] = A[1..5, 1..5];
//writeln("A[1..5,1]:",B);
for (a,b) in zip(A[1..5,1..5],F[1..5,1..5]) do if (a!=b) then {writeln("ERROR!!!!");}
var Dom1={1..5};
var Dom2={1..5,1..5};
var Dom3={1..5,1..5,1..5};

writeln();
writeln("=====================================");
writeln("Example 2");
//Dom3={1,1..5};
B = A[1,1..5];
//writeln("A[1,1..5]:",B);
for (a,b) in zip(A[1,1..5],B[1..5]) do if (a!=b) then {writeln("ERROR!!!!");}

for (a,i) in zip(B,{1..n}) do a=i;

writeln();
writeln("=====================================");
writeln("Example 3");
B=C[1,1,1..5];
//writeln("C[1,1,1..5]:", B);

for (a,b) in zip(C[1,1,1..5],B[1..5]) do if (a!=b) then {writeln("ERROR!!!!");}

for (a,i) in zip(B,{1..n}) do a=i;
writeln();
writeln("=====================================");
writeln("Example 4");
B=C[1,1..5,1];
//writeln("C[1,1..5,2]:", B);
for (a,b) in zip(C[1,1..5,1],B[1..5]) do if (a!=b) then {writeln("ERROR!!!!");}

for (a,i) in zip(B,{1..n}) do a=i;
writeln();
writeln("=====================================");
writeln("Example 5");
B=C[1..5,1,1];
//writeln("C[1..5,1,1]:", B);
for (a,b) in zip(C[1..5,1,1],B[1..5]) do if (a!=b) then {writeln("ERROR!!!!");}

for (a,i) in zip(B,{1..n}) do a=i;
writeln();
writeln("=====================================");
writeln("Example 6");
B=D[1,1,1,1..5];
//writeln("D[1,1,1,1..5]:", B);
for (a,b) in zip(D[1,1,1,1..5],B[1..5]) do if (a!=b) then {writeln("ERROR!!!!");}

for (a,i) in zip(B,{1..n}) do a=i;
writeln();
writeln("=====================================");
writeln("Example 7");
B=D[1,1,1..5,1];
//writeln("D[1,1,1..5,1]:", B);
for (a,b) in zip(D[1,1,1..5,1],B[1..5]) do if (a!=b) then {writeln("ERROR!!!!");}

for (a,i) in zip(B,{1..n}) do a=i;
writeln();
writeln("=====================================");
writeln("Example 8");
B=D[1,1..5,1,1];
//writeln("D[1,1..5,1,1]:", B);
for (a,b) in zip(D[1,1..5,1,1],B[1..5]) do if (a!=b) then {writeln("ERROR!!!!");}

for (a,i) in zip(B,{1..n}) do a=i;
writeln();
writeln("=====================================");
writeln("Example 9");
B=D[1..5,1,1,1];
//writeln("D[1..5,1,1,1]:", B);
for (a,b) in zip(D[1..5,1,1,1],B[1..5]) do if (a!=b) then {writeln("ERROR!!!!");}

for (a,i) in zip(F,{1..n*n}) do a=i;
writeln();
writeln("=====================================");
writeln("Example 10");
F=C[1,1..5,1..5];
//writeln("C[1,1..5,1..5]:", F);
for (a,b) in zip(C[1,1..5,1..5],F[1..5,1..5]) do if (a!=b) then {writeln("ERROR!!!!");}

for (a,i) in zip(F,{1..n*n}) do a=i;
writeln();
writeln("=====================================");
writeln("Example 11");
F=C[1..5,1,1..5];
//writeln("C[1..5,1,1..5]:", F);
for (a,b) in zip(C[1..5,1,1..5],F[1..5,1..5]) do if (a!=b) then {writeln("ERROR!!!!");}

for (a,i) in zip(F,{1..n*n}) do a=i;
writeln();
writeln("=====================================");
writeln("Example 12");
//F[1..4,1..4]=C[1..4,1..4,1];
//E[1..5,1..5,1]=C[1..5,1..5,1];
F=C[1..5,1..5,1];
//writeln("C:", C);
//writeln("F: ",F);
for (a,b) in zip(C[1..5,1..5,1],F[1..5,1..5]) do if (a!=b) then {writeln("ERROR!!!!");}

// All these examples would work if we remove the
// a.dom.size==b.dom.size condition from ChapelArray to do
// bulkCopy in proc chpl__useBulkTransferStride(a:[], b:[]).
/*
writeln();
for (a,i) in zip(E,{1..n*n*n}) do a=i;
writeln("=====================================");
writeln("Example 13");
Dom3={1..1,1..5,1..5};
E[1..5,1..5,1..5]=C[Dom3];
//writeln("C[1..1,1..5,1..5]:", E);
for (a,b) in zip(C[Dom3],E[Dom3]) do if (a!=b) then {writeln("ERROR!!!!");}

for (a,i) in zip(E,{1..n*n*n}) do a=i;
writeln();
writeln("=====================================");
writeln("Example 14");
Dom3={1..5,1..1,1..5};
E=C[Dom3];
//writeln("C[1..5,1..1,1..5]:", E);
for (a,b) in zip(C[Dom3],E[1..1,1..5,1..5]) do if (a!=b) then {writeln("ERROR!!!!");}

for (a,i) in zip(E,{1..n*n*n}) do a=i;
writeln();
writeln("=====================================");
writeln("Example 15");
Dom3 ={1..5,1..5,1..1};
E=C[Dom3];
//writeln("C[1..5,1..5,1..1]:", E);
for (a,b) in zip(E[1..1,1..5,1..5],C[Dom3]) do if (a!=b) then {writeln("ERROR!!!!");}
*/

