use PrintComms;

config const m = 4;
config const n = 4;
config const o = 4;

config const initialize = true;
config const printOutput = false;
config const printTiming = false;

const shift = (123, 456, 789);

var Dom = {1..m,1..n,1..o};
var e=false;

use Random, Time;

var Ref: [Dom] real;
var A: [Dom] real;
var B: [Dom] real;
for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

var Dom1={2..2,3..3,2..2};
var Dom2={1..1,2..2,2..2};

//EXAMPLE 1
for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;
//writeln("Example 1: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln("Example 1: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

Dom2={2..2,3..4,2..2};
Dom1={4..4,1..2,2..2};

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

//writeln("Example 2: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 2: ",Dom2);

  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

Dom2={2..3,4..4,2..2};
Dom1={3..4,2..2,2..2};
//writeln("Example 3: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln();
writeln("Example 3: ",Dom2);

  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;


Dom2={1..2,3..4,2..2};
Dom1={3..4,1..2,2..2};
//writeln("Example 4: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 4: ",Dom2);

  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

Dom2={1..m,4..4,2..2};
Dom1={1..m,2..2,2..2};
  //writeln("Example 4: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 5: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

Dom2={1..m,3..4,2..2};
Dom1={1..m,1..2,2..2};
  //writeln("Example 5: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 6: ",Dom2);

  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

Dom2={1..1,1..n,2..2};
Dom1={3..3,1..n,2..2};
 //writeln("Example 6: ",Dom2); 
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln();
writeln("Example 7: ",Dom2);

  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;
Dom2={1..2,1..n,2..2};
Dom1={3..4,1..n,2..2};
  //writeln("Example 7: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln();
writeln("Example 8: ",Dom2);

  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

Dom2={1..m,1..n,2..2};
Dom1={1..m,1..n,2..2};
  //writeln("Example 9: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln();
writeln("Example 9: ",Dom2);

  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

//========================================

Dom1={2..2,3..3,2..o};
Dom2={1..1,2..2,2..o};

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;
//writeln("Example 10: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 10: ",Dom2);

  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

Dom2={2..2,3..4,2..o};
Dom1={4..4,1..2,2..o};

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

//writeln("Example 11: ",Dom2);
 A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 11: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

Dom2={2..3,4..4,2..o};
Dom1={3..4,2..2,2..o};
//writeln("Example 12: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
writeln();
writeln("Example 12: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;


Dom2={1..2,3..4,2..o};
Dom1={3..4,1..2,2..o};
  //writeln("Example 13: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 13: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

Dom2={1..m,4..4,2..o};
Dom1={1..m,2..2,2..o};
  //writeln("Example 14: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 14: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

Dom2={1..m,3..4,2..o};
Dom1={1..m,1..2,2..o};
  //writeln("Example 15: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 15: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

Dom2={1..1,1..n,2..o};
Dom1={3..3,1..n,2..o};
  //writeln("Example 16: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln();
writeln("Example 16: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;
Dom2={1..2,1..n,2..o};
Dom1={3..4,1..n,2..o};
 // writeln("Example 17: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln();
writeln("Example 17: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

Dom2={1..m,1..n,2..o};
Dom1={1..m,1..n,2..o};
  //writeln("Example 18: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln();
writeln("Example 18: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

//========================================

Dom1={2..2,3..3,1..o};
Dom2={1..1,2..2,1..o};
//Dom1=Dom2;

//EXAMPLE 1

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;
//writeln("Example 19: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 19: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

Dom2={2..2,3..4,1..o};
Dom1={4..4,1..2,1..o};

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

//writeln("Example 20: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 20: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

Dom2={2..3,4..4,1..o};
Dom1={3..4,2..2,1..o};

//Dom1=Dom2;
//EXAMPLE 2
//writeln("Example 21: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln();
writeln("Example 21: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;


Dom2={1..2,3..4,1..o};
Dom1={3..4,1..2,1..o};
//  writeln("Example 22: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 22: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

Dom2={1..m,4..4,1..o};
Dom1={1..m,2..2,1..o};

  A[Dom2]=B[Dom1];
//writeln("Example 23: ",Dom2);

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 23: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

Dom2={1..m,3..4,1..o};
Dom1={1..m,1..2,1..o};
//  writeln("Example 24: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln();
writeln("Example 24: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

Dom2={1..1,1..n,1..o};
Dom1={3..3,1..n,1..o};

//  writeln("Example 25: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln();
writeln("Example 25: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;
Dom2={1..2,1..n,1..o};
Dom1={3..4,1..n,1..o};
//  writeln("Example 26: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln();
writeln("Example 26: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

for (a,i) in zip(A,{1..m*n*o}) do a = i;
for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

Dom2={1..m,1..n,1..o};
Dom1={1..m,1..n,1..o};

//  writeln("Example 27: ",Dom2);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln();
writeln("Example 27: ",Dom2);
  for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
  for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
}

writeln("");
