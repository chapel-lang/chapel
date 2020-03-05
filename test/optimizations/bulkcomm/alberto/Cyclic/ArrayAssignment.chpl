use CyclicDist;
use Time;
config const printOutput=false;

config  const n: int=20;

/*var Dist1 = new dmap(new Block({1..n,1..n,1..n}));
var Dist2 = new dmap(new Block({1..2*n,1..2*n,1..2*n}));
var Dom1: domain(3,int) dmapped Dist1 = {1..n,1..n,1..n};
var Dom2: domain(3,int) dmapped Dist2 = {1..2*n,1..2*n,1..2*n};
*/
const Space = {1..n,1..n,1..n};
const Space2={1..2*n,1..2*n,1..2*n};
const Dom1: domain(3) dmapped Cyclic(startIdx=Space.low)=Space;
const Dom2: domain(3) dmapped Cyclic(startIdx=Space2.low)=Space2;
//writeln("TasksPerLocale: ",dataParTasksPerLocale);
//Block Dist. Examples 3D
var A:[Dom1] int(64)=1;
var B:[Dom2] int(64)=500;
if printOutput
{
  writeln("Examples 3D");
  writeln("A defined on ",Dom1," and B on ",Dom2);
}

writeln();
var D1={1..n by 4,1..n by 3 ,1..n by 2};
var st,dt=getCurrentTime();
var elem=1;

if printOutput then writeln("Block Dist. Example 1: A",D1, " = B",D1," on ",numLocales," Locales:");
st = getCurrentTime();

A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in zip(A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1={1..n,1..n by 4,1..n};

if printOutput then writeln("Block Dist. Example 2: A",D1, " = B",D1," on ",numLocales," Locales:");
for (a,i) in zip(A,{1..n*n*n}) do a=i;
st = getCurrentTime();

A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);

forall (a,b) in zip(A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1={1..n by 4,1..n ,1..n by 2};
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 3: A",D1, " = B",D1," on ",numLocales," Locales:");
for (a,i) in zip(A,{1..n*n*n}) do a=i;
st = getCurrentTime();

A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in zip(A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1={1..n by 4,1..n by 3 ,1..n};
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 4: A",D1, " = B",D1," on ",numLocales," Locales:");
st = getCurrentTime();
for (a,i) in zip(A,{1..n*n*n}) do a=i;
A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in zip(A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1={1..n,1..n by 3 ,1..n by 2};
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 5: A",D1, " = B",D1," on ",numLocales," Locales:");
st = getCurrentTime();
for (a,i) in zip(A,{1..n*n*n}) do a=i;
A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in zip(A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1={1..n by 4,1..n,1..n};
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 6: A",D1, " = B",D1," on ",numLocales," Locales:");
st = getCurrentTime();
for (a,i) in zip(A,{1..n*n*n}) do a=i;
A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in zip(A[D1],B[D1]) do if (a!=b){ writeln("ERROR!!!!");}

writeln();
D1={1..n,1..n ,1..n by 2};
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 7: A",D1, " = B",D1," on ",numLocales," Locales:");
st = getCurrentTime();
for (a,i) in zip(A,{1..n*n*n}) do a=i;
A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in zip(A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1={1..n,1..n ,1..n by 2}; //001
var D2={1..n,1..n ,1..2*n by 4};
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 8: A",D1, " = B",D2," on ",numLocales," Locales:");
st = getCurrentTime();
for (a,i) in zip(A,{1..n*n*n}) do a=i;
A[D1]=B[D2];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in zip(A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1={1..n,1..n by 2,1..n by 2}; //011
D2={1..n,1..2*n by 4 ,1..2*n by 4};
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 9: A",D1, " = B",D2," on ",numLocales," Locales:");
st = getCurrentTime();
for (a,i) in zip(A,{1..n*n*n}) do a=i;
A[D1]=B[D2];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in zip(A[D1],B[D2]) do if (a!=b){ writeln("ERROR!!!!");}

writeln();
D1={1..n by 3,1..n by 2,1..n by 2}; //111
D2={1..n by 3,1..2*n by 4,1..n by 2};
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 10: A",D1, " = B",D2," on ",numLocales," Locales:");
st = getCurrentTime();
for (a,i) in zip(A,{1..n*n*n}) do a=i;
A[D1]=B[D2];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in zip(A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1={1..n by 2,1..n,1..n by 2};
D2={1..2*n by 4,1..n,1..n by 2};//101
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 11: A",D1, " = B",D2," on ",numLocales," Locales:");
st = getCurrentTime();
for (a,i) in zip(A,{1..n*n*n}) do a=i;
A[D1]=B[D2];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in zip(A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1={1..n by 2,1..n,1..n};
D2={1..2*n by 4,1..n,1..n};//100
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 12: A",D1, " = B",D2," on ",numLocales," Locales:");
st = getCurrentTime();
for (a,i) in zip(A,{1..n*n*n}) do a=i;
A[D1]=B[D2];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in zip(A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1={1..n,1..n by 2,1..n};
D2={1..n,1..2*n by 4,1..n};//010
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 13: A",D1, " = B",D2," on ",numLocales," Locales:");
st = getCurrentTime();
for (a,i) in zip(A,{1..n*n*n}) do a=i;
A[D1]=B[D2];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in zip(A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");

D1={1..n,2..n by 2,5..n-1};
D2={1..n,2..2*n by 4,4..n-2};

elem=1;
for h in 1..Dom1.rank
{
  if D2.dim(h).size != D1.dim(h).size
  {
    elem=0;
    break;
  }
}

if elem==1
{
  if printOutput then writeln("Block Dist. Example 14: A",D1, " = B",D2," on ",numLocales," Locales:");
  st=getCurrentTime();
  for (a,i) in zip(A,{1..n*n*n}) do a=i;
  A[D1]=B[D2];
  dt = getCurrentTime()-st;
  if printOutput then writeln("Time: ", dt);
  for (a,b) in zip(A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");
}

D1={1..n-1 by 3,2..n/2 by 2,5..n-1};
D2={1..2*(n-1) by 6,2..n by 4,4..n-2};

elem=1;
for h in 1..Dom1.rank
{
  if D2.dim(h).size != D1.dim(h).size
  {
    elem=0;
    break;
  }
}

if elem==1
{
  if printOutput then writeln("Block Dist. Example 15: A",D1, " = B",D2," on ",numLocales," Locales:");
  st=getCurrentTime();
  for (a,i) in zip(A,{1..n*n*n}) do a=i;
  A[D1]=B[D2];
  dt = getCurrentTime()-st;
  if printOutput then writeln("Time: ", dt);
  for (a,b) in zip(A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");
}

D1={2..n by 4,2..n/2 by 2,1..n/2 by 5};
D2={2..n/2 by 2,2..n by 4,n/4+1..3*n/4 by 5};

elem=1;
for h in 1..Dom1.rank
{
  if D2.dim(h).size != D1.dim(h).size
  {
    elem=0;
    break;
  }
}

if elem==1
{
  if printOutput then writeln("Block Dist. Example 16: A",D1, " = B",D2," on ",numLocales," Locales:");
  st=getCurrentTime();
  for (a,i) in zip(A,{1..n*n*n}) do a=i;
  A[D1]=B[D2];
  dt = getCurrentTime()-st;
  if printOutput then writeln("Time: ", dt);
  for (a,b) in zip(A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");
}


D1={1..n/2 by 3,n/2+1..3*n/4 by 2,1..n/2 by 5};
D2={n+1..2*n by 6,n/4+1..3*n/4 by 4,n/4+1..3*n/4 by 5};

elem=1;
for h in 1..Dom1.rank
{
  if D2.dim(h).size != D1.dim(h).size
  {
    elem=0;
    break;
  }
}

if elem==1
{
  if printOutput then writeln("Block Dist. Example 17: A",D1, " = B",D2," on ",numLocales," Locales:");
  st=getCurrentTime();
  for (a,i) in zip(A,{1..n*n*n}) do a=i;
  A[D1]=B[D2];
  dt = getCurrentTime()-st;
  if printOutput then writeln("Time: ", dt);
  for (a,b) in zip(A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");
}

D1={n/2+1..n by 2,1..n by 4,n/4+1..n/2 by 2};
D2={n/2+1..3*n/2 by 4,n/2+1..n by 2,n/4+1..3*n/4 by 4};

elem=1;
for h in 1..Dom1.rank
{
  if D2.dim(h).size != D1.dim(h).size
  {
    elem=0;
    break;
  }
}

if elem==1
{
  if printOutput then writeln("Block Dist. Example 18: A",D1, " = B",D2," on ",numLocales," Locales:");
  st=getCurrentTime();
  for (a,i) in zip(A,{1..n*n*n}) do a=i;
  A[D1]=B[D2];
  dt = getCurrentTime()-st;
  if printOutput then writeln("Time: ", dt);
  for (a,b) in zip(A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");
}


D1={n/4+1..3*n/4 by 2,n/2+1..n by 2,n/4+1..n/2 by 3};
D2={n+1..2*n by 4,3*n/2+1..2*n by 2,n/4+1..3*n/4 by 6};

elem=1;
for h in 1..Dom1.rank
{
  if D2.dim(h).size != D1.dim(h).size
  {
    elem=0;
    break;
  }
}

if elem==1
{
  if printOutput then writeln("Block Dist. Example 19: A",D1, " = B",D2," on ",numLocales," Locales:");
  st=getCurrentTime();
  for (a,i) in zip(A,{1..n*n*n}) do a=i;
  A[D1]=B[D2];
  dt = getCurrentTime()-st;
  if printOutput then writeln("Time: ", dt);
  for (a,b) in zip(A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");
}

D1={n/4+1..3*n/4 by 2,n/2+1..3*n/4 by 1,n/4+1..n/2 by 2};
D2={n+1..5*n/4 by 1,3*n/2+1..2*n by 2,n/4+1..3*n/4 by 4};

elem=1;
for h in 1..Dom1.rank
{
  if D2.dim(h).size != D1.dim(h).size
  {
    elem=0;
    break;
  }
}

if elem==1
{
  if printOutput then writeln("Block Dist. Example 20: A",D1, " = B",D2," on ",numLocales," Locales:");
  st=getCurrentTime();
  for (a,i) in zip(A,{1..n*n*n}) do a=i;
  A[D1]=B[D2];
  dt = getCurrentTime()-st;
  if printOutput then writeln("Time: ", dt);
  for (a,b) in zip(A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");
}

const Space3 = {1..n,1..n,1..n,1..n};
const Space4={1..2*n,1..2*n,1..2*n,1..2*n};
const Dom3: domain(4) dmapped Cyclic(startIdx=Space3.low)=Space3;
const Dom4: domain(4) dmapped Cyclic(startIdx=Space4.low)=Space4;
/*
var Dist3 = new dmap(new Block({1..n,1..n,1..n,1..n}));
var Dist4 = new dmap(new Block({1..2*n,1..2*n,1..2*n,1..2*n}));
var Dom3: domain(4,int) dmapped Dist3 = {1..n,1..n,1..n,1..n};
var Dom4: domain(4,int) dmapped Dist4 = {1..2*n,1..2*n,1..2*n,1..2*n};
*/
//Block Dist. Examples 4D
var C:[Dom3] int(64)=1;
var D:[Dom4] int(64)=500;
if printOutput
{
  writeln("Examples 4D");
  writeln("C defined on ",Dom3," and D on ",Dom4);
}
writeln();
var D3={1..n,1..n,1..n ,1..n by 2};
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 21: C",D3, " = D",D3," on ",numLocales," Locales:");
st = getCurrentTime();
for (a,i) in zip(C,{1..n*n*n*n}) do a=i;
C[D3]=D[D3];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in zip(C[D3],D[D3]) do if (a!=b) then writeln("ERROR!!!!");
