use BlockDist;
use Time;
config const printOutput=false;

config  const n: int=50;
var Dist1 = new dmap(new Block([1..n,1..n,1..n]));
var Dist2 = new dmap(new Block([1..2*n,1..2*n,1..2*n]));
var Dom1: domain(3,int) dmapped Dist1 = [1..n,1..n,1..n];
var Dom2: domain(3,int) dmapped Dist2 = [1..2*n,1..2*n,1..2*n];

//writeln("TasksPerLocale: ",dataParTasksPerLocale);
//Block Dist. Examples 3D
var A:[Dom1] int(64)=1;
var B:[Dom2] int(64)=500;

writeln();
var D1=[1..n by 4,1..n by 3 ,1..n by 2];
var st,dt=getCurrentTime();

if printOutput then writeln("Block Dist. Example 1: A",D1, " = B",D1," on ",numLocales," Locales:");
st = getCurrentTime();

A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in (A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1=[1..n,1..n by 4,1..n];

if printOutput then writeln("Block Dist. Example 2: A",D1, " = B",D1," on ",numLocales," Locales:");
st = getCurrentTime();

A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);

forall (a,b) in (A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1=[1..n by 4,1..n ,1..n by 2];
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 3: A",D1, " = B",D1," on ",numLocales," Locales:");

st = getCurrentTime();

A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in (A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1=[1..n by 4,1..n by 3 ,1..n];
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 4: A",D1, " = B",D1," on ",numLocales," Locales:");
st = getCurrentTime();

A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in (A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1=[1..n,1..n by 3 ,1..n by 2];
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 5: A",D1, " = B",D1," on ",numLocales," Locales:");
st = getCurrentTime();

A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in (A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1=[1..n by 4,1..n,1..n];
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 6: A",D1, " = B",D1," on ",numLocales," Locales:");
st = getCurrentTime();

A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in (A[D1],B[D1]) do if (a!=b){ writeln("ERROR!!!!");}

writeln();
D1=[1..n,1..n ,1..n by 2];
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 7: A",D1, " = B",D1," on ",numLocales," Locales:");
st = getCurrentTime();

A[D1]=B[D1];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in (A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1=[1..n,1..n ,1..n by 2]; //001
var D2=[1..n,1..n ,1..2*n by 4];
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 8: A",D1, " = B",D2," on ",numLocales," Locales:");
st = getCurrentTime();

A[D1]=B[D2];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in (A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1=[1..n,1..n by 2,1..n by 2]; //011
D2=[1..n,1..2*n by 4 ,1..2*n by 4];
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 9: A",D1, " = B",D2," on ",numLocales," Locales:");
st = getCurrentTime();

A[D1]=B[D2];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in (A[D1],B[D2]) do if (a!=b){ writeln("ERROR!!!!");}

writeln();
D1=[1..n by 3,1..n by 2,1..n by 2]; //111
D2=[1..n by 3,1..2*n by 4,1..n by 2];
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 10: A",D1, " = B",D2," on ",numLocales," Locales:");
st = getCurrentTime();

A[D1]=B[D2];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in (A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1=[1..n by 2,1..n,1..n by 2];
D2=[1..2*n by 4,1..n,1..n by 2];//101
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 11: A",D1, " = B",D2," on ",numLocales," Locales:");
st = getCurrentTime();

A[D1]=B[D2];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in (A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1=[1..n by 2,1..n,1..n];
D2=[1..2*n by 4,1..n,1..n];//100
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 12: A",D1, " = B",D2," on ",numLocales," Locales:");
st = getCurrentTime();

A[D1]=B[D2];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in (A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
D1=[1..n,1..n by 2,1..n];
D2=[1..n,1..2*n by 4,1..n];//010
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 13: A",D1, " = B",D2," on ",numLocales," Locales:");
st = getCurrentTime();

A[D1]=B[D2];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in (A[D1],B[D2]) do if (a!=b) then writeln("ERROR!!!!");



var Dist3 = new dmap(new Block([1..n,1..n,1..n,1..n]));
var Dist4 = new dmap(new Block([1..2*n,1..2*n,1..2*n,1..2*n]));
var Dom3: domain(4,int) dmapped Dist3 = [1..n,1..n,1..n,1..n];
var Dom4: domain(4,int) dmapped Dist4 = [1..2*n,1..2*n,1..2*n,1..2*n];

//Block Dist. Examples 4D
var C:[Dom3] int(64)=1;
var D:[Dom4] int(64)=500;

writeln();
var D3=[1..n,1..n,1..n ,1..n by 2];
st=getCurrentTime();

if printOutput then writeln("Block Dist. Example 21: C",D3, " = D",D3," on ",numLocales," Locales:");
st = getCurrentTime();

C[D3]=D[D3];

dt = getCurrentTime()-st;
if printOutput then writeln("Time: ", dt);
forall (a,b) in (C[D3],D[D3]) do if (a!=b) then writeln("ERROR!!!!");
