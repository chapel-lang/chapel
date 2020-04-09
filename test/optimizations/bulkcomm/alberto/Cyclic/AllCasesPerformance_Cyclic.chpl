use CyclicDist;
use Time;
use PrintComms;

config const n:int = 50;
config const p:int =10;
config const q:int =10;

config const printTime=false;
config const printOutput=false;
config const printComm=false;

var st,dt=getCurrentTime();
var e=false;
//writeln(" N:",n," P:",p," Q:",q);
//EXAMPLES 2d Block with and without stride
const Space = {1..n,1..n};
const d2DomB1: domain(2) dmapped Cyclic(startIdx=Space.low)=Space;
const Space2 = {1..2*n,1..2*n};
const d2DomB2: domain(2) dmapped Cyclic(startIdx=Space2.low)=Space2;

var d2A:[d2DomB1] real;
var d2C:[d2DomB1] real;
var d2B:[d2DomB2] real;

var a,b:real;
var i:int;
var d2Dom1={1..n,1..n};
var d2Dom2={1..n,1..n};
 
d2A=1;
  for (a,i) in zip(d2B,{1..n*n*2*2}) do a=100+i;
  for (a,i) in zip(d2C,{1..n*n}) do a=200+i;

if printOutput then writeln("Example 1. Block Dist: d2A = d2C");
if printComm then startCommDiagnostics();
st = getCurrentTime();
  
d2A=d2C;
  
dt = getCurrentTime()-st;
if printComm{
    stopCommDiagnostics();
    myPrintComms("");
}
if printTime then writeln("Example 1. Time elapsed: ", dt);
for (a,b) in zip(d2A[d2Dom1],d2C[d2Dom2]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();

// ==============================================================================
d2A=1;
if printOutput then writeln("Example 2. Block Dist: d2A",d2Dom1," = d2C",d2Dom2);
if printComm{
    resetCommDiagnostics();
    startCommDiagnostics();
}
  
d2A[d2Dom1]=d2C[d2Dom2];
  
if printComm{
    stopCommDiagnostics();
    myPrintComms("");
}
dt = getCurrentTime()-st;

if printTime then writeln("Example 2. Time elapsed: ", dt);
  for (a,b) in zip(d2A[d2Dom1],d2C[d2Dom2]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();

// ==============================================================================
//Reset array d2A
d2A=1;
d2Dom1={1..n ,1..n};
d2Dom2={1..n ,1..n};
if printOutput then writeln("Example 3. Block Dist: d2A",d2Dom1," = d2B",d2Dom2);
if printComm{
    resetCommDiagnostics();
    startCommDiagnostics();
}
st = getCurrentTime();
  
d2A[d2Dom1]=d2B[d2Dom2];
  
dt = getCurrentTime()-st;
if printComm{
    stopCommDiagnostics();
    myPrintComms("");
}
if printTime then writeln("Example 3. Time elapsed: ", dt);
  for (a,b) in zip(d2A[d2Dom1],d2B[d2Dom2]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();

  // ==============================================================================
  //Reset array d2A
d2A=1;
d2Dom1 ={1..n/2,n/4+1..3*n/4};
d2Dom2={1..n/2,n/4+1..3*n/4};
if printOutput then  writeln("Example 4. Block Dist: d2A",d2Dom1," = d2C",d2Dom2);
if printComm{
    resetCommDiagnostics();
    startCommDiagnostics();
}
st = getCurrentTime();
  
d2A[d2Dom1]=d2C[d2Dom2];
  
dt = getCurrentTime()-st;
if printComm{
    stopCommDiagnostics();
    myPrintComms("");
}
if printTime then writeln("Example 4. Time elapsed: ", dt);
  for (a,b) in zip(d2A[d2Dom1],d2C[d2Dom2]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();

  // ==============================================================================
  //Reset array d2A
d2A=1;
d2Dom1 ={1..n/2,n/2+1..n};
d2Dom2={1..n/2,n/4+1..3*n/4};
if printOutput then writeln("Example 5. Block Dist: d2A",d2Dom1," = d2C",d2Dom2);
if printComm{
    resetCommDiagnostics();
    startCommDiagnostics();
}
st = getCurrentTime();
  
d2A[d2Dom1]=d2C[d2Dom2];
  
dt = getCurrentTime()-st;
if printComm{
    stopCommDiagnostics();
    myPrintComms("");
}
if printTime then writeln("Example 5. Time elapsed: ", dt);
  for (a,b) in zip(d2A[d2Dom1],d2C[d2Dom2]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();

// ==============================================================================
  //Reset array d2A
d2A=1;
d2Dom2 ={1..n/2,n/2..n};
d2Dom1={1..n/2,n/4..3*n/4};
if printOutput then writeln("Example 6. Block Dist: d2A",d2Dom1," = d2B",d2Dom2);
if printComm{
    resetCommDiagnostics();
    startCommDiagnostics();
}
st = getCurrentTime();
  
d2A[d2Dom1]=d2B[d2Dom2];
  
dt = getCurrentTime()-st;
if printComm{
    stopCommDiagnostics();
    myPrintComms("");
}
if printTime then writeln("Example 6. Time elapsed: ", dt);
  for (a,b) in zip(d2A[d2Dom1],d2B[d2Dom2]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();

// ==============================================================================
  //Reset array d2A
d2A=1;
var d2Dom5 ={1..n by 2,1..n by 2};
if printOutput then writeln("Example 7. Block Dist: d2A",d2Dom5," = d2C",d2Dom5);
if printComm{
    resetCommDiagnostics();
    startCommDiagnostics();
}
st = getCurrentTime();
  
d2A[d2Dom5]=d2C[d2Dom5];
  
dt = getCurrentTime()-st;
if printComm{
    stopCommDiagnostics();
    myPrintComms("");
}
if printTime then writeln("Example 7. Time elapsed: ", dt);
  for (a,b) in zip(d2A[d2Dom5],d2C[d2Dom5]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();

// ==============================================================================
  //Reset array d2A
d2A=1;
var d2Dom6 ={2..n by 2,2..n};
if printOutput then  writeln("Example 8. Block Dist: d2A",d2Dom6," = d2B",d2Dom6);
if printComm{
    resetCommDiagnostics();
    startCommDiagnostics();
}
st = getCurrentTime();
  
d2A[d2Dom6]=d2B[d2Dom6];
  
dt = getCurrentTime()-st;
if printComm{
    stopCommDiagnostics();
    myPrintComms("");
}
if printTime then writeln("Example 8. Time elapsed: ", dt);
  for (a,b) in zip(d2A[d2Dom6],d2B[d2Dom6]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();

// ==============================================================================
  //Reset array d2A
d2A=1;
d2Dom6 ={1..n ,1..n by 2};
d2Dom5 ={1..n ,1..2*n by 4};
if printOutput then  writeln("Example 9. Block Dist:: d2A",d2Dom6," = d2B",d2Dom5);
if printComm{
    resetCommDiagnostics();
    startCommDiagnostics();
}
st = getCurrentTime();
  
d2A[d2Dom6]=d2B[d2Dom5];
  
dt = getCurrentTime()-st;
if printComm{
    stopCommDiagnostics();
    myPrintComms("");
}
if printTime then writeln("Example 9. Time elapsed: ", dt);
  for (a,b) in zip(d2A[d2Dom6],d2B[d2Dom5]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();

 // ==============================================================================
//EXAMPLES 3d Block with and stride
const Space3 = {1..p,1..p,1..p};
const d3DomB1: domain(3) dmapped Cyclic(startIdx=Space3.low)=Space3;
const Space4 = {1..2*p,1..2*p,1..2*p};
const d3DomB2: domain(3) dmapped Cyclic(startIdx=Space4.low)=Space4;

//Block Dist. Examples 3D
var d3A:[d3DomB1] int(64)=1;
var d3B:[d3DomB2] int(64)=500;

//Examples 3d with stride
var d3Dom1={1..p by 4,1..p by 3 ,1..p by 2};

if printOutput then writeln("Example 10: d3A",d3Dom1, " = d3B",d3Dom1," on ",numLocales," Locales:");
if printComm{ resetCommDiagnostics(); startCommDiagnostics();}
st = getCurrentTime();

  d3A[d3Dom1]=d3B[d3Dom1];

dt = getCurrentTime()-st;
if printComm{
  stopCommDiagnostics();
  myPrintComms("");
}
if printTime then writeln("Example 10. Time elapsed: ", dt);
for (a,b) in zip(d3A[d3Dom1],d3B[d3Dom1]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();
//***********************************************************************
for (a,i) in zip(d3A,{1..p*p*p}) do a=i;
d3Dom1={1..p,1..p by 4,1..p};

if printOutput then writeln("Example 11: d3A",d3Dom1, " = d3B",d3Dom1," on ",numLocales," Locales:");
if printComm{ resetCommDiagnostics(); startCommDiagnostics();}
st = getCurrentTime();

  d3A[d3Dom1]=d3B[d3Dom1];

dt = getCurrentTime()-st;
if printComm{
  stopCommDiagnostics();
  myPrintComms("");
}
if printTime then writeln("Example 11. Time elapsed: ", dt);
for (a,b) in zip(d3A[d3Dom1],d3B[d3Dom1]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();
//***********************************************************************
for (a,i) in zip(d3A,{1..p*p*p}) do a=i;
d3Dom1={1..p by 4,1..p ,1..p by 2};

if printOutput then writeln("Example 12: d3A",d3Dom1, " = d3B",d3Dom1," on ",numLocales," Locales:");
if printComm{ resetCommDiagnostics(); startCommDiagnostics();}
st = getCurrentTime();

  d3A[d3Dom1]=d3B[d3Dom1];

dt = getCurrentTime()-st;
if printComm{
  stopCommDiagnostics();
  myPrintComms("");
}
if printTime then writeln("Example 12. Time elapsed: ", dt);
for (a,b) in zip(d3A[d3Dom1],d3B[d3Dom1]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();
//***********************************************************************
for (a,i) in zip(d3A,{1..p*p*p}) do a=i;
d3Dom1={1..p by 4,1..p by 3 ,1..p};

if printOutput then writeln("Example 13: d3A",d3Dom1, " = d3B",d3Dom1," on ",numLocales," Locales:");
if printComm{ resetCommDiagnostics(); startCommDiagnostics();}
st = getCurrentTime();

  d3A[d3Dom1]=d3B[d3Dom1];

dt = getCurrentTime()-st;
if printComm{
  stopCommDiagnostics();
  myPrintComms("");
}
if printTime then writeln("Example 13. Time elapsed: ", dt);
for (a,b) in zip(d3A[d3Dom1],d3B[d3Dom1]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();
//***********************************************************************
for (a,i) in zip(d3A,{1..p*p*p}) do a=i;
d3Dom1={1..p,1..p by 3 ,1..p by 2};

if printOutput then writeln("Example 14: d3A",d3Dom1, " = d3B",d3Dom1," on ",numLocales," Locales:");
if printComm{ resetCommDiagnostics(); startCommDiagnostics();}
st = getCurrentTime();

  d3A[d3Dom1]=d3B[d3Dom1];

dt = getCurrentTime()-st;
if printComm{
  stopCommDiagnostics();
  myPrintComms("");
}
if printTime then writeln("Example 14. Time elapsed: ", dt);
for (a,b) in zip(d3A[d3Dom1],d3B[d3Dom1]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();
//***********************************************************************
for (a,i) in zip(d3A,{1..p*p*p}) do a=i;
d3Dom1={1..p by 4,1..p,1..p};

if printOutput then writeln("Example 15: d3A",d3Dom1, " = d3B",d3Dom1," on ",numLocales," Locales:");
if printComm{ resetCommDiagnostics(); startCommDiagnostics();}
st = getCurrentTime();

  d3A[d3Dom1]=d3B[d3Dom1];

dt = getCurrentTime()-st;
if printComm{
  stopCommDiagnostics();
  myPrintComms("");
}
if printTime then writeln("Example 15. Time elapsed: ", dt);
for (a,b) in zip(d3A[d3Dom1],d3B[d3Dom1]) do if (a!=b){ writeln("ERROR!!!!");}
writeln();
//***********************************************************************
for (a,i) in zip(d3A,{1..p*p*p}) do a=i;
d3Dom1={1..p,1..p ,1..p by 2};
var d3Dom4={1..p,1..p ,1..2*p by 4};

if printOutput then writeln("Example 16: d3A",d3Dom1, " = d3B",d3Dom4," on ",numLocales," Locales:");
if printComm{ resetCommDiagnostics(); startCommDiagnostics();}
st = getCurrentTime();

  d3A[d3Dom1]=d3B[d3Dom4];

dt = getCurrentTime()-st;
if printComm{
  stopCommDiagnostics();
  myPrintComms("");
}
if printTime then writeln("Example 16. Time elapsed: ", dt);
for (a,b) in zip(d3A[d3Dom1],d3B[d3Dom4]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();
//***********************************************************************
for (a,i) in zip(d3A,{1..p*p*p}) do a=i;
d3Dom1={1..p by 2,1..p,1..p};
d3Dom4={1..2*p by 4,1..p,1..p};

if printOutput then writeln("Example 17: d3A",d3Dom1, " = d3B",d3Dom4," on ",numLocales," Locales:");
if printComm{ resetCommDiagnostics(); startCommDiagnostics();}
st = getCurrentTime();

  d3A[d3Dom1]=d3B[d3Dom4];

dt = getCurrentTime()-st;
if printComm{
  stopCommDiagnostics();
  myPrintComms("");
}
if printTime then writeln("Example 17. Time elapsed: ", dt);
for (a,b) in zip(d3A[d3Dom1],d3B[d3Dom4]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();
//***********************************************************************
//Example 4d Block with stride
const Space5 = {1..q,1..q,1..q,1..q};
const d4DomB1: domain(4) dmapped Cyclic(startIdx=Space5.low)=Space5;
const Space6 = {1..2*q,1..2*q,1..2*q,1..2*q};
const d4DomB2: domain(4) dmapped Cyclic(startIdx=Space6.low)=Space6;

//Block Dist. Examples 4D
var d4C:[d4DomB1] int(64)=1;
var d4D:[d4DomB2] int(64)=500;

var d4Dom_1={1..q,1..q,1..q ,1..q by 2};

if printOutput then writeln("Example 18: d4C",d4Dom_1, " = d4D",d4Dom_1," on ",numLocales," Locales:");
if printComm{ resetCommDiagnostics(); startCommDiagnostics();}
st = getCurrentTime();

  d4C[d4Dom_1]=d4D[d4Dom_1];

dt = getCurrentTime()-st;
if printComm{
  stopCommDiagnostics();
  myPrintComms("");
}
if printTime then writeln("Example 18. Time elapsed: ", dt);
for (a,b) in zip(d4C[d4Dom_1],d4D[d4Dom_1]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();
//***********************************************************************
d4Dom_1={1..q,1..q,1..q by 2,1..q};
if printOutput then writeln("Example 19: d4C",d4Dom_1, " = d4D",d4Dom_1," on ",numLocales," Locales:");
if printComm{ resetCommDiagnostics(); startCommDiagnostics();}
st = getCurrentTime();

  d4C[d4Dom_1]=d4D[d4Dom_1];

dt = getCurrentTime()-st;
if printComm{
  stopCommDiagnostics();
  myPrintComms("");
}
if printTime then writeln("Example 19. Time elapsed: ", dt);
for (a,b) in zip(d4C[d4Dom_1],d4D[d4Dom_1]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
writeln();
//***********************************************************************
d4Dom_1={1..q,1..q by 2,1..q,1..q};
if printOutput then writeln("Example 20: d4C",d4Dom_1, " = d4D",d4Dom_1," on ",numLocales," Locales:");
if printComm{ resetCommDiagnostics(); startCommDiagnostics();}
st = getCurrentTime();

  d4C[d4Dom_1]=d4D[d4Dom_1];

dt = getCurrentTime()-st;
if printComm{
  stopCommDiagnostics();
  myPrintComms("");
}
if printTime then writeln("Example 20. Time elapsed: ", dt);
writeln();
for (a,b) in zip(d4C[d4Dom_1],d4D[d4Dom_1]) do if (a!=b) {writeln("ERROR!!!!");e=true;}

//***********************************************************************
d4Dom_1={1..q by 2,1..q,1..q,1..q};
if printOutput then writeln("Example 21: d4C",d4Dom_1, " = d4D",d4Dom_1," on ",numLocales," Locales:");
if printComm{ resetCommDiagnostics(); startCommDiagnostics();}
st = getCurrentTime();

  d4C[d4Dom_1]=d4D[d4Dom_1];

dt = getCurrentTime()-st;
if printComm{
  stopCommDiagnostics();
  myPrintComms("");
}
if printTime then writeln("Example 21. Time elapsed: ", dt);

for (a,b) in zip(d4C[d4Dom_1],d4D[d4Dom_1]) do if (a!=b) {writeln("ERROR!!!!");e=true;}
