use CyclicDist;
use BlockDist;

use Time;
use PrintComms;
config const doDiagnostics=false;
config const printOutput=true;
config const printComm=false;

config  const n: int=10;

const Space2 = {1..n,1..n};
const Dom2: domain(2) dmapped Cyclic(startIdx=Space2.low)=Space2;
const Space3 = {1..n,1..n,1..n};
const Dom3: domain(3) dmapped Cyclic(startIdx=Space3.low)=Space3;

var Dist2 = new dmap(new Block({1..n,1..n}));
var Dom2B: domain(2,int) dmapped Dist2 = {1..n,1..n};
var Dist3 = new dmap(new Block({1..n,1..n,1..n}));
var Dom3B: domain(3,int) dmapped Dist3 = {1..n,1..n,1..n};


var A2:[Dom2] real;
var A3:[Dom3] real;
var BD:[Dom2B] real;
var BD3:[Dom3B] real;

proc main(){

  var a,b:real;
  var i:int;
  var D1={1..n by 1,1..n by 1};
  var D2={1..n by 1,1..n by 1};
  var D3={1..n by 1,1..n by 1,1..n by 1};
  var D4={1..n by 1,1..n by 1,1..n by 1};
  
  var st,dt=getCurrentTime();
  for (a,i) in zip(A2,{1..n*n}) do a=i;
//2D Examples
// ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A2,{1..n*n}) do a=i;
  for (a,i) in zip(BD,{1..n*n}) do a=i+100;
  D2 ={1..10 by 1,1..10 by 1};
  D1={1..10 by 1 ,1..10 by 1};
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
  D2 ={1..n/2 by 2,1..n};
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
  D2 ={2..n/2 +1,1..n/2 -1};
  D1={n/2+1..n,n/2+2..n};
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
  for (a,i) in zip(BD,{1..n*n}) do a=i+100;
  D2 ={3..n-2 by 2,1..n/2 by 2};
  D1={4..n-1 by 2,1..n by 4};
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 5:BD",D2," = CY",D1);
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
  D1 ={2..n-1 by 2,1..n/2 by 2};
  D2={3..n by 2,1..n by 4};
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 6:BD",D2," = CY",D1);
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
  D1 ={1..n by 2,2..(n-1)/2};
  D2={1..n by 2,3..n-2 by 2};
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 7:BD",D2," = CY",D1);
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
  D2 ={2..n-1 by 2,3..n/2};
  D1={3..n by 2,4..n-1 by 2};
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 8:BD",D2," = CY",D1);
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
  D1 ={2..n/2,2..n/2};
  D2={2..n-1 by 2 ,2..n/2};
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 9:BD",D2," = CY",D1);
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
  D2 ={5..n/2,2..(n/2)-1};
  D1={7..n-2 by 2 ,3..n/2};
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 10:BD",D2," = CY",D1);
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
  
  //3D Examples
// ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A3,{1..n*n*n}) do a=i;
  for (a,i) in zip(BD3,{1..n*n*n}) do a=i+100;
  D3 ={2..10 by 1,2..9 by 1,7..10};
  D4={1..9 by 1 ,3..10 by 1,3..6};
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 11:BD",D4," = CY",D3);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  BD3[D4]=A3[D3];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A3[D3],BD3[D4]) do if (a!=b){ writeln("ERROR!!!!"); break;}

// ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A3,{1..n*n*n}) do a=i;
  for (a,i) in zip(BD3,{1..n*n*n}) do a=i+100;
  D3 ={1..n/2 by 2,1..n,3..n/2+2};
  D4={1..n by 4 ,1..n,1..n/2};
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 12:BD",D4," = CY",D3);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  BD3[D4]=A3[D3];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A3[D3],BD3[D4]) do if (a!=b){ writeln("ERROR!!!!"); break;}
  // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A3,{1..n*n*n}) do a=i;
  for (a,i) in zip(BD3,{1..n*n*n}) do a=i+100;
  D4 ={1..n/2 by 2,2..n-1,3..n/2+3};
  D3={1..n by 4 ,3..n,2..n/2+2};

  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 13:BD",D4," = CY",D3);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  BD3[D4]=A3[D3];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A3[D3],BD3[D4]) do if (a!=b){ writeln("ERROR!!!!"); break;}
  
  // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A3,{1..n*n*n}) do a=i;
  for (a,i) in zip(BD3,{1..n*n*n}) do a=i+100;
  D4 ={2..n by 2,3..n/2 by 2,1..n/4};
  D3={1..n-1 by 2,5..n by 4,1..n/4};
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 14:BD",D4," = CY",D3);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  BD3[D4]=A3[D3];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A3[D3],BD3[D4]) do if (a!=b){ writeln("ERROR!!!!"); break;}
  // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A3,{1..n*n*n}) do a=i;
  for (a,i) in zip(BD3,{1..n*n*n}) do a=i+100;
  D3 ={6..n-1 by 2,1..n/2 by 2,5..n/4-2};
  D4={7..n by 2,1..n by 4,7..n/4};

  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 15:BD",D4," = CY",D3);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  BD3[D4]=A3[D3];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A3[D3],BD3[D4]) do if (a!=b){ writeln("ERROR!!!!"); break;}

  // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A3,{1..n*n*n}) do a=i;
  for (a,i) in zip(BD3,{1..n*n*n}) do a=i+100;
  D3 ={1..n-1 by 2,1..n/2 by 2,3..n-2 by 2};
  D4={2..n by 2,1..n by 4,5..n by 2};

  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 16:BD",D4," = CY",D3);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  BD3[D4]=A3[D3];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A3[D3],BD3[D4]) do if (a!=b){ writeln("ERROR!!!!"); break;}
  // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A3,{1..n*n*n}) do a=i;
  for (a,i) in zip(BD3,{1..n*n*n}) do a=i+100;
    D4 ={1..n by 2,1..n/2 by 2,1..n by 2};
  D3={1..n by 2,1..n by 4,1..n by 2};

  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 17:BD",D4," = CY",D3);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  BD3[D4]=A3[D3];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A3[D3],BD3[D4]) do if (a!=b){ writeln("ERROR!!!!"); break;}
  
  // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A3,{1..n*n*n}) do a=i;
  for (a,i) in zip(BD3,{1..n*n*n}) do a=i+100;
  D4 ={1..n by 2,1..n/2,1..n/2};
  D3={1..n by 2,1..n by 2,1..n by 2};

  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 18:BD",D4," = CY",D3);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  BD3[D4]=A3[D3];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A3[D3],BD3[D4]) do if (a!=b){ writeln("ERROR!!!!"); break;}
  // ==============================================================================
 
  //Reset array A
  for (a,i) in zip(A3,{1..n*n*n}) do a=i;
  for (a,i) in zip(BD3,{1..n*n*n}) do a=i+100;
  D3 ={1..n by 2,1..n/2,1..n/2};
  D4={1..n by 2,1..n by 2,1..n by 2};
  
  if printOutput then writeln(" Block Dist <-- Cyclic Dist. Example 19:BD",D4," = CY",D3);
  if doDiagnostics {
    if printComm{
      resetCommDiagnostics();
      startCommDiagnostics();
    }
  }
  st = getCurrentTime();
  BD3[D4]=A3[D3];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    if printComm{
      stopCommDiagnostics();
      myPrintComms("");
    }
    writeln("Time: ", dt);
  }

  for (a,b) in zip(A3[D3],BD3[D4]) do if (a!=b){ writeln("ERROR!!!!"); break;}


}
