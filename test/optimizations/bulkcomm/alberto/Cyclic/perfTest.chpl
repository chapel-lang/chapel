use CyclicDist;
use BlockDist;

use Time;
use PrintComms;
config const doDiagnostics=false;
//config const debugBulkTransfer=true;
config const printOutput=false;
config const printComm=false;

config  const n: int=100;

const Space = {1..n,1..n,1..n};
const Space2 = {1..2*n,1..2*n,1..2*n};
const Dom1: domain(3) dmapped Cyclic(startIdx=Space.low)=Space;
const Dom2: domain(3) dmapped Cyclic(startIdx=Space2.low)=Space2;
const Space3 = {1..n,1..n};
const Space4 = {1..2*n,1..2*n};
const Dom3: domain(2) dmapped Cyclic(startIdx=Space3.low)=Space3;
const Dom4: domain(2) dmapped Cyclic(startIdx=Space4.low)=Space4;


//var Dist1 = new dmap(new Block({1..n,1..n,1..n}));
//var Dist2 = new dmap(new Block({1..(2*n),1..(2*n),1..(2*n)}));
//var Dom1: domain(3,int) dmapped Dist1 = {1..n,1..n,1..n};
//var Dom2: domain(3,int) dmapped Dist2 = {1..2*n,1..2*n,1..2*n};

var Dist3 = new dmap(new Block({1..n,1..n}));
//var Dist4 = new dmap(new Block({1..(2*n),1..(2*n)}));
var Dom3B: domain(2,int) dmapped Dist3 = {1..n,1..n};
//var Dom4: domain(2,int) dmapped Dist4 = {1..2*n,1..2*n};

var A2:[Dom3] real;
var C2:[Dom3] real;
var B2:[Dom4] real;
var BD:[Dom3B] real;
var ADR: [1..n,1..n] real;
var BDR: [1..n,1..n] real;

proc main(){

  var a,b:real;
  var i:int;
  var D1={1..n by 1,1..n by 1};
  var D2={1..n by 1,1..n by 1};
  var st,dt=getCurrentTime();
  for (a,i) in zip(A2,{1..n*n}) do a=i;

  if printOutput then writeln("Cyclic Dist. Example 1:");
  if doDiagnostics then startCommDiagnostics();
  A2=C2;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
  }
 
  for (a,i) in zip(A2,{1..n*n}) do a=i;

  if printOutput then writeln("Cyclic Dist. Example 1b: A",D1," = B",D2);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D1]=C2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A2[D1],C2[D2]) do if (a!=b){ writeln("ERROR!!!!");break;}
  
  // ==============================================================================
  
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (b,i) in zip(B2,{1..2*n*2*n}) do b=i+100.0;
  D1={1..n ,1..n};
  D2={1..n ,1..n};
  if printOutput then writeln("Cyclic Dist. Example 2:A",D1," = C",D2);
  if doDiagnostics then startCommDiagnostics();
  st = getCurrentTime();
  A2[D1]=B2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
  
  for (a,b) in zip(A2[D1],B2[D2]) do if (a!=b){ writeln("ERROR!!!!");break;}
  // ==============================================================================
  
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;

  D1 ={1..n/2,n/4+1..3*n/4};
  D2={1..n/2,n/4+1..3*n/4};

  if printOutput then  writeln("Cyclic Dist. Example 3:A",D1," = C",D2);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D1]=C2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
  
  for (a,b) in zip(A2[D1],C2[D2]) do if (a!=b){ writeln("ERROR!!!!");break;}
  
  // ==============================================================================
  
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  D1 ={1..n/2,n/2+1..n};
  D2={1..n/2,n/4+1..3*n/4};

  if printOutput then writeln("Cyclic Dist. Example 4:A",D1," = C",D2);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D1]=C2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
  for (a,b) in zip(A2[D1],C2[D2]) do if (a!=b){ writeln("ERROR!!!!");break;}

  // ==============================================================================
  
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  D2 ={1..n/2,n/2..n};
  D1={1..n/2,n/4..3*n/4};

  if printOutput then writeln("Cyclic Dist. Example 5:A",D1," = C",D2);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D1]=B2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
  
  for (a,b) in zip(A2[D1],B2[D2]) do if (a!=b){ writeln("ERROR!!!!");break;}
 
  // ==============================================================================
  
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  var D6 ={2..10 by 2,2..10};
  C2[1..n,1..n]=999;
  if printOutput then  writeln("Cyclic Dist. Example 6:A",D6," = C",D6);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D6]=C2[D6];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
  //writeln("A2: ", A2);
  //writeln("C2: ", C2);
  for (a,b) in zip(A2[D6],C2[D6]) do if (a!=b){ writeln("ERROR!!!!");break;}
  
  // ==============================================================================
  
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  var D7 ={1..n by 2,1..n by 2};

  if printOutput then writeln("Cyclic Dist. Example 7:A",D7," = C",D7);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D7]=C2[D7];
  // writeln("A2: ", A2);
  // writeln("C2: ", C2);
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
  
  for (a,b) in zip(A2[D7],C2[D7]) do if (a!=b){ writeln("ERROR!!!!");break;}
  
  // ==============================================================================
  
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  var D8 ={1..n by 3,1..n by 2};
  if printOutput then writeln("Cyclic Dist. Example 8:A",D8," = C",D8);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D8]=C2[D8];
  //  writeln("A2: ", A2);
  //  writeln("C2: ", C2);
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
 
  for (a,b) in zip(A2[D8],C2[D8]) do if (a!=b){ writeln("ERROR!!!!");break;}

  // ==============================================================================

  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  D6 ={2..n by 2,2..n};

  if printOutput then  writeln("Cyclic Dist. Example 9:A",D6," = B",D6);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D6]=B2[D6];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A2[D6],B2[D6]) do if (a!=b){ writeln("ERROR!!!!");break;}
  
  // ==============================================================================

  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  D6 ={2..n by 2,2..n};
  D7 ={1..n-1 by 2,1..n-1};
  //C2[1..n,1..n]=999;
  if printOutput then  writeln("Cyclic Dist. Example 10:A",D6," = B",D7);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D6]=B2[D7];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
  //writeln("A2: ", A2);
  //writeln("C2: ", C2);
  for (a,b) in zip(A2[D6],B2[D7]) do if (a!=b){ writeln("ERROR!!!!");break;}
  
  // ==============================================================================

  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  D6 ={2..n by 20,2..n};
  D7 ={1..n-1 by 20,1..n-1};

  if printOutput then  writeln("Cyclic Dist. Example 11:A",D6," = B",D7);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D6]=B2[D7];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
  //writeln("A2: ", A2);
  //writeln("C2: ", C2);
  for (a,b) in zip(A2[D6],B2[D7]) do if (a!=b){ writeln("ERROR!!!!");break;}

  // ==============================================================================

  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  D6 ={1..n ,1..n by 2};
  D7 ={1..n ,1..2*n by 4};

  if printOutput then  writeln("Cyclic Dist. Example 12:A",D6," = B",D7);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D6]=B2[D7];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
  //writeln("A2: ", A2);
  //writeln("C2: ", C2);
  for (a,b) in zip(A2[D6],B2[D7]) do if (a!=b){ writeln("ERROR!!!!");break;}

  // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(BD,{1..n*n}) do a=i+100;
  D2 ={1..n by 3,1..n by 4};
  D1={1..n by 3 ,1..n by 4};
  if printOutput then writeln("Cyclic Dist <-- Block Dist. Example 1:A",D1," = BD",D2);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D1]=BD[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A2[D1],BD[D2]) do if (a!=b){ writeln("ERROR!!!!");break;}
  
 
  // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(BD,{1..n*n}) do a=i+100;
  D2 ={1..n/2 by 2,1..n };
  D1={1..n by 4 ,1..n};
  if printOutput then writeln("Cyclic Dist <-- Block Dist. Example 2:A",D1," = BD",D2);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D1]=BD[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A2[D1],BD[D2]) do if (a!=b){ writeln("ERROR!!!!");break;}
 
 // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(BD,{1..n*n}) do a=i+100;
  D2 ={1..n,1..n/2 by 2};
  D1={1..n,1..n by 4};
  if printOutput then writeln("Cyclic Dist <-- Block Dist. Example 3:A",D1," = BD",D2);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D1]=BD[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A2[D1],BD[D2]) do if (a!=b){ writeln("ERROR!!!!");break;}
 
 // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(BD,{1..n*n}) do a=i+100;
  D2 ={1..n/2,1..n/2};
  D1={n/2+1..n,n/2+1..n};
  if printOutput then writeln("Cyclic Dist <-- Block Dist. Example 4:A",D1," = BD",D2);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D1]=BD[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A2[D1],BD[D2]) do if (a!=b){ writeln("ERROR!!!!");break;}
 
 // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(BD,{1..n*n}) do a=i+100;
  D2 ={1..10 by 3,1..10 by 4};
  D1={1..10 by 3 ,1..10 by 4};
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 1:BD",D2," = CY",D1);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  BD[D2]=A2[D1];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A2[D1],BD[D2]) do if (a!=b){ writeln("ERROR!!!!"); break;}
  
 
  // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(BD,{1..n*n}) do a=i+100;
  D2 ={1..n/2 by 2,1..n };
  D1={1..n by 4 ,1..n};
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 2:BD",D2," = CY",D1);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  BD[D2]=A2[D1];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A2[D1],BD[D2]) do if (a!=b){ writeln("ERROR!!!!"); break;}
 
 // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(BD,{1..n*n}) do a=i+100;
  D2 ={1..n,1..n/2 by 2};
  D1={1..n,1..n by 4};
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 3:BD",D2," = CY",D1);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  BD[D2]=A2[D1];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A2[D1],BD[D2]) do if (a!=b){ writeln("ERROR!!!!"); break;}
 
 // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(BD,{1..n*n}) do a=i+100;
  D2 ={1..n/2,1..n/2};
  D1={n/2+1..n,n/2+1..n};
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 4:BD",D2," = CY",D1);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  BD[D2]=A2[D1];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A2[D1],BD[D2]) do if (a!=b){ writeln("ERROR!!!!"); break;}
 
 // ==============================================================================
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(ADR,{1..n*n}) do a=i+100;
  D2 ={1..n by 3,1..n by 4};
  D1={1..n by 3 ,1..n by 4};
  if printOutput then writeln("Cyclic Dist <-- Default Rectangular. Example 1:A",D1," = ADR",D2);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D1]=ADR[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A2[D1],ADR[D2]) do if (a!=b){ writeln("ERROR!!!!"); break;}
  
  // ==============================================================================
  
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(ADR,{1..n*n}) do a=i+100;

  D1={1..n/2,n/4..3*n/4};
  D2 = D1;
  if printOutput then writeln("Cyclic Dist <-- Default Rectangular. Example 2:A",D1," = ADR",D2);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D1]=ADR[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
 
  for (a,b) in zip(A2[D1],ADR[D2]) do if (a!=b){ writeln("ERROR!!!!"); break;}
  

  // ==============================================================================
  
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(ADR,{1..n*n}) do a=i+100;

  D1={1..n/2,1..n/2};
  D2 ={1..n/2,n/2+1..n};
  if printOutput then writeln("Cyclic Dist <-- Default Rectangular. Example 3:A",D2," = ADR",D1);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  A2[D2]=ADR[D1];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
  
  for (a,b) in zip(A2[D2],ADR[D1]) do if (a!=b){ writeln("ERROR!!!!"); break;}
 
  // ==============================================================================
  
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(ADR,{1..n*n}) do a=i+100;
  D2 ={1..n,1..n};
  D1={1..n,1..n};
  if printOutput then writeln("Default Rectangular <-- Cyclic Dist. Example 1:ADR",D1," = A",D2);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  ADR[D1]=A2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
 
  for (a,b) in zip(A2[D2],ADR[D1]) do if (a!=b){ writeln("ERROR!!!!"); break;}
  
  // ==============================================================================
  
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(ADR,{1..n*n}) do a=i+100;

  D1={1..n/2,n/4..3*n/4};
  D2 = D1;
  if printOutput then writeln("Default Rectangular <-- Cyclic Dist. Example 2:ADR",D1," = A",D2);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  ADR[D2]=A2[D1];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
 
  for (a,b) in zip(A2[D1],ADR[D2]) do if (a!=b){ writeln("ERROR!!!!"); break;}
  
  // ==============================================================================
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(ADR,{1..n*n}) do a=i+100;

  D1={1..n/2,1..n/2};
  D2 ={1..n/2,n/2+1..n};
  if printOutput then writeln("Default Rectangular <-- Cyclic Dist. Example 3:ADR",D2," = A",D1);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  ADR[D1]=A2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }
  
  for (a,b) in zip(A2[D2],ADR[D1]) do if (a!=b){ writeln("ERROR!!!!"); break;}
  
  // ============================================================================== 
 
  D1={1..n,n/4+1..3*n/4};
  D2=D1;
  if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 1 :BDR",D1,"(L0) = ADR",D2,"(L0) on L0");

  //var D:[D1] real;
  
  for (a,i) in zip(BDR,{1..n*n}) do a=i+100;
  for (a,i) in zip(ADR,{1..n*n}) do a=i;
  
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  ADR[D1] = BDR[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  
  }
 
  for (a,b) in zip(BDR[D2],ADR[D1]) do if (a!=b){ writeln("ERROR!!!!"); break;}
  
  // ==============================================================================
  
  //  writeln("Default Rectangular <-- Default Rectangular. Example 2 :ADR",D1,"(L0) = BDR",D2,"(L0) on Locale 1");
  for (a,i) in zip(ADR,{1..n*n}) do a=i;
  D1={1..n,1..n};
  D2=D1;
  if(numLocales<2){
    if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 2: Not enough number of Locales ",numLocales," (2 needed)");
  }
  else{
  on Locales(1)
    {
      if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 2 :ADR",D1,"(",ADR.locale,") = BDR",D2,"(",BDR.locale,") on ", here.locale);  
      if doDiagnostics {
	if printComm{
	  resetCommDiagnostics();
	  startCommDiagnostics();
	}
      }
      st = getCurrentTime();
  
      ADR[D1] = BDR[D2];
  
      dt = getCurrentTime()-st;
      if doDiagnostics {
	if printComm{
	  stopCommDiagnostics();
	  myPrintComms("");
	}
	writeln("Time: ", dt);
      }
  for (a,b) in zip(BDR[D2],ADR[D1]) do if (a!=b){ writeln("ERROR!!!!"); break;}
    }
  }
 
  
  
  // ==============================================================================
  //  D1={1..n,n/4+1..3*n/4};
  D2=D1;
  var mes:string;
  if(numLocales<3){
    if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 3: Not enough number of Locales ",numLocales," (3 needed)");
  }
  else{
    if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 3 :ADR",D1,"(L0) = BDR",D2,"(L2) on Locale 1");
    for (a,i) in zip(ADR,{1..n*n}) do a=i;

    on Locales(2)
      {
  
	var CDR: [1..n,1..n] real;
	for (a,i) in zip(CDR,{1..n*n}) do a=i+100;
	if doDiagnostics {
	  if printComm{
	    resetCommDiagnostics();
	    startCommDiagnostics();
	  }
	}
  
	st = getCurrentTime();
  
	on Locales(1){
	  ADR[D1] = CDR[D2];
	}
  
	dt = getCurrentTime()-st;
	if doDiagnostics {
	  if printComm{
	    stopCommDiagnostics();
	    myPrintComms("");
	  }
	  writeln("Time: ", dt);
	}
	for (a,b) in zip(CDR[D2],ADR[D1]) do if (a!=b){ writeln("ERROR!!!!");break;}
      }
  } 

  // ==============================================================================


  if printOutput then  writeln("Default Rectangular <-- Default Rectangular. Example 4 :ADR",D1,"(L0) = CDR",D2,"(L1) on Locale 1"); 
  for (a,i) in zip(ADR,{1..n*n}) do a=i;

  //for (a,i) in zip(CDR,{1..n*n}) do a=i+100;
  if(numLocales<2){
    if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 4: Not enough number of Locales ",numLocales," (2 needed)");
  }
  else
  {
    on Locales(1)
    {

      var CDR: [1..n,1..n] real;
      for (a,i) in zip(CDR,{1..n*n}) do a=i+100;
      if doDiagnostics {
	if printComm{
	  resetCommDiagnostics();
	  startCommDiagnostics();
	}
      }

      st = getCurrentTime();

      //  on Locales(1){ 
      ADR[D1] = CDR[D2];
      //  }

      dt = getCurrentTime()-st;
      if doDiagnostics {
	if printComm{
	  stopCommDiagnostics();
	  myPrintComms("");
	}
	writeln("Time: ", dt);
      }
      for (a,b) in zip(CDR[D2],ADR[D1]) do if (a!=b){ writeln("ERROR!!!!");break;}
    }
  }
  // ==============================================================================

  if printOutput then  writeln("Default Rectangular <-- Default Rectangular. Example 5 :ADR",D1,"(L1) = BDR",D2,"(L0) on Locale 1"); 
  for (a,i) in zip(ADR,{1..n*n}) do a=i;

  //var CCDR:[1..n,1..n] real;
  for (a,i) in zip(ADR,{1..n*n}) do a=i+100;
 
  if(numLocales<2){
    if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 5: Not enough number of Locales ",numLocales," (2 needed)");
  }
  else
  {
    on Locales(1)
    {

      var CDR: [1..n,1..n] real;
      for (a,i) in zip(CDR,{1..n*n}) do a=i;
      if doDiagnostics {
	if printComm{
	  resetCommDiagnostics();
	  startCommDiagnostics();
	}
      }

      st = getCurrentTime();

      //  on Locales(1){ 
      CDR[D1] = ADR[D2];
      //  }

      dt = getCurrentTime()-st;
      if doDiagnostics {
	if printComm{
	  stopCommDiagnostics();
	  myPrintComms("");
	}
	writeln("Time: ", dt);
      }
      for (a,b) in zip(CDR[D1],ADR[D2]) do if (a!=b){ writeln("ERROR!!!!");break;}
    }
  }
  // ==============================================================================
 
  var D3 = {1..10, 1..5};
  //var D3 = [2..5, 2..5};
  var H: [D3] real;
  var G: [D3] real;
  if printOutput then  writeln("Default Rectangular <-- Default Rectangular. Example 6 :ADR",D3,"(L0) = CDR",D3,"(L1) on Locale 1");
  // for (a,i) in zip(ADR,{1..n*n}) do a=i;

  // on Locales(1)
  // {
  //var G: [D3] real;
  //  var CDR: [1..n,1..n] real;
  //  for (a,i) in zip(G,{1..n*n}) do a=i+100;
  //  if doDiagnostics {
  //    resetCommDiagnostics();
  //    startCommDiagnostics();
  //  }

  //  st = getCurrentTime();

  //  on Locales(1){
  //ADR[D3] = CDR[D3];
  //H[2..5 by 3,2..5] = CDR[D3];
      
  H[D3] = G[D3];
  //  }

  //  dt = getCurrentTime()-st;
  //  if doDiagnostics {
  //    stopCommDiagnostics();
  //    myPrintComms("");
  //    writeln("Time: ", dt);
  //  }
  for (a,b) in zip(G[D3],H[D3]) do if (a!=b){ writeln("ERROR!!!!");break;}
  // }
  // ==============================================================================
  //D1 = [1..n, 1..n by 2};
  //D2=D1;

  if printOutput then   writeln("Default Rectangular <-- Default Rectangular. Example 7 :ADR",D3,"(L1) = BDR",D3,"(L0) on Locale 1");
  for (a,i) in zip(ADR,{1..n*n}) do a=i;

  //var CDR:[1..n,1..n] real;
  for (a,i) in zip(ADR,{1..n*n}) do a=i+100;
   if(numLocales<2){
    if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 7: Not enough number of Locales ",numLocales," (2 needed)");
  }
  else
  {
    on Locales(1)
    {

      var CDR: [1..n,1..n] real;
      for (a,i) in zip(CDR,{1..n*n}) do a=i;
      if doDiagnostics {
	resetCommDiagnostics();
	startCommDiagnostics();
      }   

      st = getCurrentTime();

      //  on Locales(1){
      CDR[D3] = ADR[D3];
      //  }

      dt = getCurrentTime()-st;
      if doDiagnostics {
	stopCommDiagnostics();
	myPrintComms("");
	writeln("Time: ", dt);
      }
      for (a,b) in zip(CDR[D3],ADR[D3]) do if (a!=b){ writeln("ERROR!!!!");break;}
    }
  }

  // ==============================================================================

  var D4 = {1..n by 2, 1..n by 2};
  if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 8 :ADR",D4,"(L0) = CDR",D4,"(L1) on Locale 1");
  for (a,i) in zip(ADR,{1..n*n}) do a=i;
  
  if(numLocales<2){
    if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 8: Not enough number of Locales ",numLocales," (2 needed)");
  }
  else
  {
    on Locales(1)
    {

      var CDR: [1..n,1..n] real;
      for (a,i) in zip(CDR,{1..n*n}) do a=i+100;
      if doDiagnostics {
	resetCommDiagnostics();
	startCommDiagnostics();
      }

      st = getCurrentTime();

      //  on Locales(1){
      ADR[D4] = CDR[D4];
      //  }

      dt = getCurrentTime()-st;
      if doDiagnostics {
	stopCommDiagnostics();
	myPrintComms("");
	writeln("Time: ", dt);
      }
      for (a,b) in zip(CDR[D4],ADR[D4]) do if (a!=b){ writeln("ERROR!!!!");break;}
    }
  }

  // ==============================================================================

  if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 9 :ADR",D4,"(L1) = BDR",D4,"(L0) on Locale 1");
  for (a,i) in zip(ADR,{1..n*n}) do a=i;

  for (a,i) in zip(ADR,{1..n*n}) do a=i+100;
   if(numLocales<2){
    if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 9: Not enough number of Locales ",numLocales," (2 needed)");
  }
  else
  {
    on Locales(1)
    {

      var CDR: [1..n,1..n] real;
      for (a,i) in zip(CDR,{1..n*n}) do a=i;
      if doDiagnostics {
	resetCommDiagnostics();
	startCommDiagnostics();
      }

      st = getCurrentTime();

      //  on Locales(1){
      CDR[D4] = ADR[D4];
      //  }

      dt = getCurrentTime()-st;
      if doDiagnostics {
	stopCommDiagnostics();
	myPrintComms("");
	writeln("Time: ", dt);
      }
      for (a,b) in zip(CDR[D4],ADR[D4]) do if (a!=b){ writeln("ERROR!!!!");break;}
    }
  }

  // ==============================================================================

  var D5 = {1..n by 10, 1..n};

  if printOutput then  writeln("Default Rectangular <-- Default Rectangular. Example 10 :ADR",D5,"(L0) = CDR",D5,"(L1) on Locale 1");
  for (a,i) in zip(ADR,{1..n*n}) do a=i;

 if(numLocales<2){
    if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 10: Not enough number of Locales ",numLocales," (2 needed)");
  }
  else
  {
    on Locales(1)
    {

      var CDR: [1..n,1..n] real;
      for (a,i) in zip(CDR,{1..n*n}) do a=i+100;
      if doDiagnostics {
	resetCommDiagnostics();
	startCommDiagnostics();
      }

      st = getCurrentTime();

      //  on Locales(1){
      ADR[D5] = CDR[D5];
      //  }

      dt = getCurrentTime()-st;
      if doDiagnostics {
	stopCommDiagnostics();
	myPrintComms("");
	writeln("Time: ", dt);
      }
      for (a,b) in zip(CDR[D5],ADR[D5]) do if (a!=b){ writeln("ERROR!!!!");break;}
    }
  }
  // ==============================================================================

  if printOutput then  writeln("Default Rectangular <-- Default Rectangular. Example 11 :ADR",D5,"(L1) = BDR",D5,"(L0) on Locale 1");
  for (a,i) in zip(ADR,{1..n*n}) do a=i;

  //var CDR:[1..n,1..n] real;
  for (a,i) in zip(ADR,{1..n*n}) do a=i+100;
  if(numLocales<2){
    if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 11: Not enough number of Locales ",numLocales," (2 needed)");
  }
  else
  {
    on Locales(1)
    {

      var CDR: [1..n,1..n] real;
      for (a,i) in zip(CDR,{1..n*n}) do a=i;
      if doDiagnostics {
	resetCommDiagnostics();
	startCommDiagnostics();
      }

      st = getCurrentTime();

      //  on Locales(1){
      CDR[D5] = ADR[D5];
      //  }

      dt = getCurrentTime()-st;
      if doDiagnostics {
	stopCommDiagnostics();
	myPrintComms("");
	writeln("Time: ", dt);
      }
      for (a,b) in zip(CDR[D5],ADR[D5]) do if (a!=b){ writeln("ERROR!!!!");break;}
    }
  }
writeln();  
  // ============================================================================== 
}
