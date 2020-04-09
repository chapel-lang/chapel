config const m = 8;
config const n = 8;
config const o = 8;
config const p = 8;
config const q = 8;

config const initialize = true;
config const printOutput = false;

var Dom = {1..m,1..n,1..o,1..p,1..q};
var e=false;

var A: [Dom] real;
var B: [Dom] real;
for (a,i) in zip(A,{1..m*n*o*p*q}) do a = i;
for (b,i) in zip(B,{1..m*n*o*p*q}) do b = i+500;

var Dom1={2..2,3..3,2..2,1..3,1..2};
var Dom2={1..1,2..2,2..2,2..4,2..3};

//EXAMPLE 1
for (a,i) in zip(A,{1..m*n*o*p*q}) do a = i;
for (b,i) in zip(B,{1..m*n*o*p*q}) do b = i+500;
if printOutput then writeln("Example 1: A",Dom2," = B",Dom1);
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

var Dom3,Dom4={1..m by 2,1..n by 3,1..o by 4,1..p by 5,1..q by 6};

for (a,i) in zip(A,{1..m*n*o*p*q}) do a = i;
for (b,i) in zip(B,{1..m*n*o*p*q}) do b = i+500;
if printOutput then writeln("Example 2: A",Dom3," = B",Dom4);
  A[Dom3]=B[Dom4];

for (a,b) in zip(A[Dom3],B[Dom3]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};


Dom3={1..m by 6,1..n by 5,1..o by 4,1..p by 3,1..q by 2};
Dom4 = Dom3;

for (a,i) in zip(A,{1..m*n*o*p*q}) do a = i;
for (b,i) in zip(B,{1..m*n*o*p*q}) do b = i+500;
if printOutput then writeln("Example 3: A",Dom3," = B",Dom4);
  A[Dom3]=B[Dom4];

for (a,b) in zip(A[Dom3],B[Dom3]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

Dom3={1..m by 4,1..n by 5,1..o by 2,1..p by 3,1..q by 6};
Dom4 = Dom3;

for (a,i) in zip(A,{1..m*n*o*p*q}) do a = i;
for (b,i) in zip(B,{1..m*n*o*p*q}) do b = i+500;
if printOutput then writeln("Example 4: A",Dom3," = B",Dom4);
  A[Dom3]=B[Dom4];

for (a,b) in zip(A[Dom3],B[Dom3]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

Dom3={1..m-1 by 4,1..n-1 by 3,1..o-1 by 3,1..p-1 by 3,1..q-1 by 2};
Dom4 = Dom3;

for (a,i) in zip(A,{1..m*n*o*p*q}) do a = i;
for (b,i) in zip(B,{1..m*n*o*p*q}) do b = i+500;
if printOutput then writeln("Example 5: A",Dom3," = B",Dom4);
  A[Dom3]=B[Dom4];

for (a,b) in zip(A[Dom3],B[Dom3]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

Dom3={1..m by 4,1..n by 5,1..o by 2,1..p by 3,1..q by 2};
Dom4 = Dom3;

for (a,i) in zip(A,{1..m*n*o*p*q}) do a = i;
for (b,i) in zip(B,{1..m*n*o*p*q}) do b = i+500;
if printOutput then writeln("Example 6: A",Dom3," = B",Dom4);
  A[Dom3]=B[Dom4];

for (a,b) in zip(A[Dom3],B[Dom3]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

Dom3={2..m by 4,2..n by 3,2..o by 3,2..p by 3,2..q by 2};
Dom4 = Dom3;

for (a,i) in zip(A,{1..m*n*o*p*q}) do a = i;
for (b,i) in zip(B,{1..m*n*o*p*q}) do b = i+500;
if printOutput then writeln("Example 7: A",Dom3," = B",Dom4);
  A[Dom3]=B[Dom4];

for (a,b) in zip(A[Dom3],B[Dom3]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

Dom3={2..m-1 by 4,2..n-1 by 3,2..o-1 by 3,2..p-1 by 3,2..q-1 by 2};
Dom4 = Dom3;

for (a,i) in zip(A,{1..m*n*o*p*q}) do a = i;
for (b,i) in zip(B,{1..m*n*o*p*q}) do b = i+500;
if printOutput then writeln("Example 8: A",Dom3," = B",Dom4);
  A[Dom3]=B[Dom4];

for (a,b) in zip(A[Dom3],B[Dom3]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

Dom3={2..m-1 by 4,2..n-1 by 5,2..o-1 by 6,2..p-1 by 2,2..q-1 by 3};
Dom4 = Dom3;

for (a,i) in zip(A,{1..m*n*o*p*q}) do a = i;
for (b,i) in zip(B,{1..m*n*o*p*q}) do b = i+500;
if printOutput then writeln("Example 9: A",Dom3," = B",Dom4);
  A[Dom3]=B[Dom4];

for (a,b) in zip(A[Dom3],B[Dom3]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

Dom3={1..m by 4,1..n by 5,1..o by 6,1..p by 2,1..q by 3};
Dom4 ={1..m by 6,1..n by 4,1..o by 7,1..p by 2,1..q/2 +1 by 2};

for (a,i) in zip(A,{1..m*n*o*p*q}) do a = i;
for (b,i) in zip(B,{1..m*n*o*p*q}) do b = i+500;
if printOutput then writeln("Example 10: A",Dom3," = B",Dom4);
  A[Dom3]=B[Dom4];

for (a,b) in zip(A[Dom3],B[Dom4]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

Dom3={1..m/2 by 2,3..n by 5,1..o/2 by 2,1..p by 4,1..q by 3};
Dom4 ={1..m by 4,1..n by 4,2..o-1 by 3,1..p/2 by 2,1..q/2 +1 by 2};

for (a,i) in zip(A,{1..m*n*o*p*q}) do a = i;
for (b,i) in zip(B,{1..m*n*o*p*q}) do b = i+500;
if printOutput then writeln("Example 11: A",Dom3," = B",Dom4);
  A[Dom3]=B[Dom4];

for (a,b) in zip(A[Dom3],B[Dom4]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if e then writeln("Hey man!!!!... double check it! There was an ERROR!");
writeln("");
