use PrintComms;

config const m = 4;//6;
config const n = 4;//3;

config const initialize = true;
config const printOutput = false;
config const printTiming = false;

const shift = (123, 456, 789);
var Dom = {1..m,1..n};
var e=false;

use Random, Time;

var Ref: [Dom] real;
var A: [Dom] real;
var B: [Dom] real;

var Dom1={2..2,3..3};
var Dom2={1..1,2..2};

//EXAMPLE 1

for (a,i) in zip(A,{1..m*n}) do a = i;
for (b,i) in zip(B,{1..m*n}) do b = i+500;

  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln("Example 1: A",Dom2," = B",Dom1);
  for (i,j) in Dom do writeln("A[",i,",", j,"] = ",A[i,j]);
  for (i,j) in Dom do writeln("B[",i,",", j,"] = ",B[i,j]);
}

Dom2={2..2,3..4};
Dom1={4..4,1..2};

for (a,i) in zip(A,{1..m*n}) do a = i;
for (b,i) in zip(B,{1..m*n}) do b = i+500;

  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln("Example 2: A",Dom2," = B",Dom1);
  for (i,j) in Dom do writeln("A[",i,",", j,"] = ",A[i,j]);
  for (i,j) in Dom do writeln("B[",i,",", j,"] = ",B[i,j]);
}

Dom2={2..3,4..4};
Dom1={3..4,2..2};

  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
writeln("Example 3: A",Dom2," = B",Dom1);
  for (i,j) in Dom do writeln("A[",i,",", j,"] = ",A[i,j]);
  for (i,j) in Dom do writeln("B[",i,",", j,"] = ",B[i,j]);
}

for (a,i) in zip(A,{1..m*n}) do a = i;
for (b,i) in zip(B,{1..m*n}) do b = i+500;

Dom2={1..2,3..4};
Dom1={3..4,1..2};
  
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln("Example 4: A",Dom2," = B",Dom1);
  for (i,j) in Dom do writeln("A[",i,",", j,"] = ",A[i,j]);
  for (i,j) in Dom do writeln("B[",i,",", j,"] = ",B[i,j]);
}

for (a,i) in zip(A,{1..m*n}) do a = i;
for (b,i) in zip(B,{1..m*n}) do b = i+500;

Dom2={1..m,4..4};
Dom1={1..m,2..2};
  
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln("Example 5: A",Dom2," = B",Dom1);
  for (i,j) in Dom do writeln("A[",i,",", j,"] = ",A[i,j]);
  for (i,j) in Dom do writeln("B[",i,",", j,"] = ",B[i,j]);
}

for (a,i) in zip(A,{1..m*n}) do a = i;
for (b,i) in zip(B,{1..m*n}) do b = i+500;

Dom2={1..m,3..4};
Dom1={1..m,1..2};
  
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

if printOutput
{
  writeln("Example 6: A",Dom2," = B",Dom1);
  for (i,j) in Dom do writeln("A[",i,",", j,"] = ",A[i,j]);
  for (i,j) in Dom do writeln("B[",i,",", j,"] = ",B[i,j]);
}

for (a,i) in zip(A,{1..m*n}) do a = i;
for (b,i) in zip(B,{1..m*n}) do b = i+500;

Dom2={1..1,1..n};
Dom1={3..3,1..n};
  
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln("Example 7: A",Dom2," = B",Dom1);
  for (i,j) in Dom do writeln("A[",i,",", j,"] = ",A[i,j]);
  for (i,j) in Dom do writeln("B[",i,",", j,"] = ",B[i,j]);
}

for (a,i) in zip(A,{1..m*n}) do a = i;
for (b,i) in zip(B,{1..m*n}) do b = i+500;

Dom2={1..2,1..n};
Dom1={3..4,1..n};
  
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln("Example 8: A",Dom2," = B",Dom1);
  for (i,j) in Dom do writeln("A[",i,",", j,"] = ",A[i,j]);
  for (i,j) in Dom do writeln("B[",i,",", j,"] = ",B[i,j]);
}

for (a,i) in zip(A,{1..m*n}) do a = i;
for (b,i) in zip(B,{1..m*n}) do b = i+500;

Dom2={1..m,1..n};
Dom1={1..m,1..n};
  
  A[Dom2]=B[Dom1];

for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
if printOutput
{
  writeln("Example 9: A",Dom2," = B",Dom1);
  for (i,j) in Dom do writeln("A[",i,",", j,"] = ",A[i,j]);
  for (i,j) in Dom do writeln("B[",i,",", j,"] = ",B[i,j]);
}
writeln("");
