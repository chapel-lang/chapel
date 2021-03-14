use BlockDist;
config const printOutput=false;


config  const n: int=100;
var Dist1 = new dmap(new Block({1..n}));
var Dist2 = new dmap(new Block({1..n,1..n}));
var Dist3 = new dmap(new Block({1..n,1..n,1..n}));
var Dom1: domain(1,int) dmapped Dist1 = {1..n};
var Dom2: domain(2,int) dmapped Dist2 = {1..n,1..n};
var Dom3: domain(3,int) dmapped Dist3 = {1..n,1..n,1..n};

/*Default Rectangular. Examples 1D*/
var ADR:[1..n] int(64)=[i in 1..n] i;
var BDR:[1..n] int(64)=[i in 1..n] i + 499;
var DDR1 ={1..n};

if printOutput then writeln("Default Rectangular. Example 1: ADR",DDR1, " blk: ", ADR._value.blk);
ADR[DDR1]=BDR[DDR1];

var DDR2={1..n by 2};

if printOutput then writeln("Default Rectangular. Example 2: ADR",DDR2, " blk: ", ADR._value.blk);

/*Default Rectangular. Examples 2D*/
var ADR2:[1..n,1..n] int(64)=[(i,j) in {1..n,1..n}] (i-1)*n + j;
var BDR2:[1..n,1..n] int(64)=[(i,j) in {1..n,1..n}] (i-1)*n + j + 499;

var DDR3 ={1..n,1..n};

if printOutput then writeln("Default Rectangular. Example 3: ADR",DDR3, " blk: ", ADR2._value.blk);
ADR2[DDR3]=BDR2[DDR3];
for (a,b) in zip(ADR2[DDR3],BDR2[DDR3]) do if (a!=b) then writeln("ERROR!!!!");
  

var DDR4 ={1..n,1..n by 2};

if printOutput then writeln("Default Rectangular. Example 4: ADR",DDR4, " blk: ", ADR2._value.blk);
ADR2[DDR4]=BDR2[DDR4];
for (a,b) in zip(ADR2[DDR4],BDR2[DDR4]) do if (a!=b) then writeln("ERROR!!!!");
 
//DDR3[1]=1..n by 3;
//DDR3[2]=1..n by 1;
var DDR5={1..n by 3,1..n};

if printOutput then writeln("Default Rectangular. Example 5: ADR",DDR5, " blk: ", ADR2._value.blk );
ADR2[DDR5]=BDR2[DDR5];
for (a,b) in zip(ADR2[DDR5],BDR2[DDR5]) do if (a!=b) then writeln("ERROR!!!!");
  
var DDR6={1..n by 3,1..n by 2};

if printOutput then writeln("Default Rectangular. Example 6: ADR",DDR6, " blk: ", ADR2._value.blk);
ADR2[DDR6]=BDR2[DDR6];
for (a,b) in zip(ADR2[DDR6],BDR2[DDR6]) do if (a!=b) then writeln("ERROR!!!!");
  

//Default Rectangular. Examples 3D
var ADR3:[1..n,1..n,1..n] int(64)=[(i,j,k) in {1..n,1..n,1..n}]
                                    (i-1)*n*n + (j-1)*n + k;
var BDR3:[1..n,1..n,1..n] int(64)=[(i,j,k) in {1..n,1..n,1..n}]
                                    (i-1)*n*n + (j-1)*n + k + 499;

var DDR7={1..n,1..n,1..n};

if printOutput then writeln("Default Rectangular. Example 7: ADR",DDR7, " blk: ", ADR3._value.blk);
ADR3[DDR7]=BDR3[DDR7];
for (a,b) in zip(ADR3[DDR7],BDR3[DDR7]) do if (a!=b) then writeln("ERROR!!!!");

//DDR4[3]=1..n by 2;
var DDR8={1..n,1..n,1..n by 2};

if printOutput then writeln("Default Rectangular. Example 8: ADR",DDR8, " blk: ", ADR3._value.blk);
ADR3[DDR8]=BDR3[DDR8];
for (a,b) in zip(ADR3[DDR8],BDR3[DDR8]) do if (a!=b) then writeln("ERROR!!!!");

var DDR9={1..n,1..n by 3,1..n};

if printOutput then writeln("Default Rectangular. Example 9: ADR",DDR9, " blk: ", ADR3._value.blk);
ADR3[DDR9]=BDR3[DDR9];
for (a,b) in zip(ADR3[DDR9],BDR3[DDR9]) do if (a!=b) then writeln("ERROR!!!!");

var DDR10={1..n by 4, 1..n,1..n};

if printOutput then writeln("Default Rectangular. Example 10: ADR",DDR10, " blk: ", ADR3._value.blk);
ADR3[DDR10]=BDR3[DDR10];
for (a,b) in zip(ADR3[DDR10],BDR3[DDR10]) do if (a!=b) then writeln("ERROR!!!!");

var DDR11={1..n,1..n by 3,1..n by 2};

if printOutput then writeln("Default Rectangular. Example 11: ADR",DDR11, " blk: ", ADR3._value.blk);
ADR3[DDR11]=BDR3[DDR11];
for (a,b) in zip(ADR3[DDR11],BDR3[DDR11]) do if (a!=b) then writeln("ERROR!!!!");

var DDR12={1..n by 4,1..n,1..n by 2};

if printOutput then writeln("Default Rectangular. Example 12: ADR",DDR12, " blk: ", ADR3._value.blk);
ADR3[DDR12]=BDR3[DDR12];
for (a,b) in zip(ADR3[DDR12],BDR3[DDR12]) do if (a!=b) then writeln("ERROR!!!!");

var DDR13={1..n by 4,1..n by 3,1..n};

if printOutput then writeln("Default Rectangular. Example 13: ADR",DDR13, " blk: ", ADR3._value.blk);
ADR3[DDR13]=BDR3[DDR13];
for (a,b) in zip(ADR3[DDR13],BDR3[DDR13]) do if (a!=b) then writeln("ERROR!!!!");

var ADR4:[1..6,1..6,1..6] int(64)=[(i,j,k) in {1..6,1..6,1..6}]
                                    (i-1)*6*6 + (j-1)*6 + k;
var BDR4:[1..6,1..6,1..6] int(64)=[(i,j,k) in {1..6,1..6,1..6}]
                                    (i-1)*6*6 + (j-1)*6 + k + 499;
var DDR14={1..5 by 2,1..3, 1..6};

if printOutput then writeln("Default Rectangular. Example Rafa: ADR",DDR14, " blk: ", ADR4._value.blk);
ADR4[DDR14]=BDR4[DDR14];
for (a,b) in zip(ADR4[DDR14],BDR4[DDR14]) do if (a!=b) then writeln("ERROR!!!!");

/*Block Dist. Examples 1D*/
var A:[Dom1] int(64)=[i in Dom1] i;
var B:[Dom1] int(64)=[i in Dom1] i + 499;
if printOutput then writeln("Block Dist. Example 1: A",Dom1, " Locales:",numLocales);
A[Dom1]=B[Dom1];
for (a,b) in zip(A[Dom1],B[Dom1]) do if (a!=b) then writeln("ERROR!!!!");

var D1 ={1..n by 2};
A=1; 

if printOutput then writeln("Block Dist. Example 2: A",D1, " Locales:",numLocales);
A[D1]=B[D1];
for (a,b) in zip(A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

D1={1..n by n};
A=1;

if printOutput then writeln("Block Dist. Example 3: A",D1, " Locales:",numLocales);
A[D1]=B[D1];
for (a,b) in zip(A[D1],B[D1]) do if (a!=b) then writeln("ERROR!!!!");

//Block Dist. Examples 2D
var A2:[Dom2] int(64)=[(i,j) in Dom2] (i-1)*n + j;
var B2:[Dom2] int(64)=[(i,j) in Dom2] (i-1)*n + j + 499;

if printOutput then writeln("Block Dist. Example 4: A",Dom2, " Locales:",numLocales);
A2[Dom2]=B2[Dom2];
for (a,b) in zip(A2[Dom2],B2[Dom2]) do if (a!=b) then writeln("ERROR!!!!");

var D2 ={1..n, 1..n by 2};
A2=1;

if printOutput then writeln("Block Dist. Example 5: A",D2, " Locales:",numLocales);
A2[D2]=B2[D2];
for (a,b) in zip(A2[D2],B2[D2]) do if (a!=b) then writeln("ERROR!!!!");

D2 ={1..n by 3, 1..n};
A2=1;

if printOutput then writeln("Block Dist. Example 6: A",D2, " Locales:",numLocales);
A2[D2]=B2[D2];
for (a,b) in zip(A2[D2],B2[D2]) do if (a!=b) then writeln("ERROR!!!!");

D2 ={1..n by 3, 1..n by 2};
A2=1;

if printOutput then writeln("Block Dist. Example 7: A",D2, " Locales:",numLocales);
A2[D2]=B2[D2];
for (a,b) in zip(A2[D2],B2[D2]) do if (a!=b) then writeln("ERROR!!!!");

//Block Dist. Examples 3D
var A3:[Dom3] int(64)=[(i,j,k) in Dom3] (i-1)*n*n + (j-1)*n + k;
var B3:[Dom3] int(64)=[(i,j,k) in Dom3] (i-1)*n*n + (j-1)*n + k + 499;
A3=1;


if printOutput then writeln("Block Dist. Example 8: A",Dom3, " Locales:",numLocales);
A3[Dom3]=B3[Dom3];
for (a,b) in zip(A3[Dom3],B3[Dom3]) do if (a!=b) then writeln("ERROR!!!!");

var D3 ={1..n,1..n, 1..n by 2};
A3=1;

if printOutput then writeln("Block Dist. Example 9: A",D3, " Locales:",numLocales);
A3[D3]=B3[D3];
for (a,b) in zip(A3[D3],B3[D3]) do if (a!=b) then writeln("ERROR!!!!");

D3 ={1..n,1..n by 3, 1..n};
A3=1;

if printOutput then writeln("Block Dist. Example 10: A",D3, " Locales:",numLocales);
A3[D3]=B3[D3];
for (a,b) in zip(A3[D3],B3[D3]) do if (a!=b) then writeln("ERROR!!!!");

D3 ={1..n by 4, 1..n,1..n};
A3=1;

if printOutput then writeln("Block Dist. Example 11: A",D3, " Locales:",numLocales);
A3[D3]=B3[D3];
for (a,b) in zip(A3[D3],B3[D3]) do if (a!=b) then writeln("ERROR!!!!");

D3 ={1..n by 4, 1..n by 3,1..n};
A3=1;

if printOutput then writeln("Block Dist. Example 12: A",D3, " Locales:",numLocales);
A3[D3]=B3[D3];
for (a,b) in zip(A3[D3],B3[D3]) do if (a!=b) then writeln("ERROR!!!!");

D3 ={1..n by 4, 1..n,1..n by 2};
A3=1;

if printOutput then writeln("Block Dist. Example 13: A",D3, " Locales:",numLocales);
A3[D3]=B3[D3];
for (a,b) in zip(A3[D3],B3[D3]) do if (a!=b) then writeln("ERROR!!!!");


D3 ={1..n by 4, 1..n by 3,1..n};
A3=1;

if printOutput then writeln("Block Dist. Example 14: A",D3, " Locales:",numLocales);
A3[D3]=B3[D3];
for (a,b) in zip(A3[D3],B3[D3]) do if (a!=b) then writeln("ERROR!!!!");

var Dist4 = new dmap(new Block({1..6,1..6,1..6}));
var Dom4: domain(3,int) dmapped Dist4 ={1..6,1..6,1..6};
var D4 ={1..5 by 2,1..3,1..6};
var A4:[Dom4] int(64)=[(i,j,k) in Dom4] (i-1)*6*6 + (j-1)*6 + k;
var B4:[Dom4] int(64)=[(i,j,k) in Dom4] (i-1)*6*6 + (j-1)*6 + k + 499;

if printOutput then writeln("Block Dist. Example Rafa: A",D4, " Locales:",numLocales);
A4[D4]=B4[D4];
for (a,b) in zip(A4[D4],B4[D4]) do if (a!=b) then writeln("ERROR!!!!");
