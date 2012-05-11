use BlockDist;
use Time;
use PrintComms;
config const doDiagnostics=false;
//config const debugBulkTransfer=true;
config const printOutput=false;

config  const n: int=10;
var Dist1 = new dmap(new Block([1..n,1..n,1..n]));
var Dist2 = new dmap(new Block([1..(2*n),1..(2*n),1..(2*n)]));
var Dom1: domain(3,int) dmapped Dist1 = [1..n,1..n,1..n];
var Dom2: domain(3,int) dmapped Dist2 = [1..2*n,1..2*n,1..2*n];
var Dist3 = new dmap(new Block([1..n,1..n]));
var Dist4 = new dmap(new Block([1..(2*n),1..(2*n)]));
var Dom3: domain(2,int) dmapped Dist3 = [1..n,1..n];
var Dom4: domain(2,int) dmapped Dist4 = [1..2*n,1..2*n];

var A2:[Dom3] real;
var C2:[Dom3] real;
var B2:[Dom4] real;

var ADR: [1..n,1..n] real;
var BDR: [1..n,1..n] real;

proc main(){


  var a,b:real;
  var i:int;
  var D1=[1..n,1..n];
  var D2=[1..n,1..n];
  var st,dt=getCurrentTime();
  for (a,i) in (A2,[1..n*n]) do a=i;
  
  writeln();if printOutput then writeln("Block Dist. Example 1:");
  if doDiagnostics then startCommDiagnostics();
  A2=C2;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
  }
 
   for (a,i) in (A2,[1..n*n]) do a=i;

  writeln();if printOutput then writeln("Block Dist. Example 1b: A",D1," = B",D2);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  A2[D1]=C2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }

  for (a,b) in (A2[D1],C2[D2]) do if (a!=b) then writeln("ERROR!!!!");
  
  // ==============================================================================
  
  for (a,i) in (A2,[1..n*n]) do a=i;
  for (b,i) in (B2,[1..2*n*2*n]) do b=i+100.0;
  D1=[1..n ,1..n];
  D2=[1..n ,1..n];
  writeln();if printOutput then writeln("Block Dist. Example 2:A",D1," = C",D2);
  if doDiagnostics then startCommDiagnostics();
  st = getCurrentTime();
  A2[D1]=B2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
  
  for (a,b) in (A2[D1],B2[D2]) do if (a!=b) then writeln("ERROR!!!!");
 // ==============================================================================
  
   //Reset array A
  for (a,i) in (A2,[1..n*n]) do a=i;

D1 = [1..n/2,n/4..3*n/4];
D2=[1..n/2,n/4..3*n/4];

 writeln();if printOutput then  writeln("Block Dist. Example 3:A",D1," = C",D2);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  A2[D1]=C2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
  
  for (a,b) in (A2[D1],C2[D2]) do if (a!=b) then writeln("ERROR!!!!");
  
// ==============================================================================
  
   //Reset array A
  for (a,i) in (A2,[1..n*n]) do a=i;
D1 = [1..n/2,n/2..n];
D2=[1..n/2,n/4..3*n/4];

writeln();if printOutput then writeln("Block Dist. Example 4:A",D1," = C",D2);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  A2[D1]=C2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
  for (a,b) in (A2[D1],C2[D2]) do if (a!=b) then writeln("ERROR!!!!");
  
 // ==============================================================================
  
   //Reset array A
  for (a,i) in (A2,[1..n*n]) do a=i;
D2 = [1..n/2,n/2..n];
D1=[1..n/2,n/4..3*n/4];

  writeln();if printOutput then writeln("Block Dist. Example 5:A",D1," = C",D2);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  A2[D1]=B2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
  
  for (a,b) in (A2[D1],B2[D2]) do if (a!=b) then writeln("ERROR!!!!");
 
  // ==============================================================================
  
   //Reset array A
  for (a,i) in (A2,[1..n*n]) do a=i;
var D6 = [2..10 by 2,2..10];
C2[1..n,1..n]=999;
writeln();if printOutput then  writeln("Block Dist. Example 6:A",D6," = C",D6);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  A2[D6]=C2[D6];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
//writeln("A2: ", A2);
//writeln("C2: ", C2);
  for (a,b) in (A2[D6],C2[D6]) do if (a!=b) then writeln("ERROR!!!!");
  
  // ==============================================================================
  
   //Reset array A
  for (a,i) in (A2,[1..n*n]) do a=i;
var D7 = [1..n by 2,1..n by 2];

  writeln();if printOutput then writeln("Block Dist. Example 7:A",D7," = C",D7);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  A2[D7]=C2[D7];
 // writeln("A2: ", A2);
 // writeln("C2: ", C2);
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
  
  for (a,b) in (A2[D7],C2[D7]) do if (a!=b) then writeln("ERROR!!!!");
  
  
  // ==============================================================================
  
   //Reset array A
  for (a,i) in (A2,[1..n*n]) do a=i;
var D8 = [1..n by 3,1..n by 2];
  writeln();if printOutput then writeln("Block Dist. Example 8:A",D8," = C",D8);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  A2[D8]=C2[D8];
//  writeln("A2: ", A2);
//  writeln("C2: ", C2);
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
 
  for (a,b) in (A2[D8],C2[D8]) do if (a!=b) then writeln("ERROR!!!!");
  
 
  // ==============================================================================
  
   //Reset array A
  for (a,i) in (A2,[1..n*n]) do a=i;
  for (a,i) in (ADR,[1..n*n]) do a=i+100;
D2 = [1..n,1..n];
D1=[1..n,1..n];
writeln();if printOutput then writeln("Block Dist <-- Default Rectangular. Example 1:A",D1," = ADR",D2);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  A2[D1]=ADR[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }

  for (a,b) in (A2[D1],ADR[D2]) do if (a!=b) then writeln("ERROR!!!!");
  
  // ==============================================================================
  
   //Reset array A
  for (a,i) in (A2,[1..n*n]) do a=i;
  for (a,i) in (ADR,[1..n*n]) do a=i+100;

D1=[1..n/2,n/4..3*n/4];
D2 = D1;
  writeln();if printOutput then writeln("Block Dist <-- Default Rectangular. Example 2:A",D1," = ADR",D2);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  A2[D1]=ADR[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
 
  for (a,b) in (A2[D1],ADR[D2]) do if (a!=b) then writeln("ERROR!!!!");
  

  // ==============================================================================
  
   //Reset array A
  for (a,i) in (A2,[1..n*n]) do a=i;
  for (a,i) in (ADR,[1..n*n]) do a=i+100;

D1=[1..n/2,1..n/2];
D2 =[1..n/2,n/2+1..n];
  writeln();if printOutput then writeln("Block Dist <-- Default Rectangular. Example 3:A",D2," = ADR",D1);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  A2[D2]=ADR[D1];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
  
  for (a,b) in (A2[D2],ADR[D1]) do if (a!=b) then writeln("ERROR!!!!");
 
  // ==============================================================================
  
   //Reset array A
  for (a,i) in (A2,[1..n*n]) do a=i;
  for (a,i) in (ADR,[1..n*n]) do a=i+100;
D2 = [1..n,1..n];
D1=[1..n,1..n];
  writeln();if printOutput then writeln("Default Rectangular <-- Block Dist. Example 1:A",D1," = ADR",D2);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  ADR[D1]=A2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
 
  for (a,b) in (A2[D2],ADR[D1]) do if (a!=b) then writeln("ERROR!!!!");
  
  // ==============================================================================
  
   //Reset array A
  for (a,i) in (A2,[1..n*n]) do a=i;
  for (a,i) in (ADR,[1..n*n]) do a=i+100;

D1=[1..n/2,n/4..3*n/4];
D2 = D1;
  writeln();if printOutput then writeln("Default Rectangular <-- Block Dist. Example 2:A",D1," = ADR",D2);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  ADR[D2]=A2[D1];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
 
  for (a,b) in (A2[D1],ADR[D2]) do if (a!=b) then writeln("ERROR!!!!");
  
  // ==============================================================================
   //Reset array A
  for (a,i) in (A2,[1..n*n]) do a=i;
  for (a,i) in (ADR,[1..n*n]) do a=i+100;

D1=[1..n/2,1..n/2];
D2 =[1..n/2,n/2+1..n];
 writeln();if printOutput then writeln("Default Rectangular <-- Block Dist. Example 3:A",D2," = ADR",D1);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  ADR[D1]=A2[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
  
  for (a,b) in (A2[D2],ADR[D1]) do if (a!=b) then writeln("ERROR!!!!");
  
 // ============================================================================== 
  
  D1=[1..n,n/4+1..3*n/4];
  D2=D1;
  writeln();if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 1 :BDR",D1,"(L0) = ADR",D2,"(L0) on L0");

  //var D:[D1] real;
  
  for (a,i) in (BDR,[1..n*n]) do a=i+100;
  for (a,i) in (ADR,[1..n*n]) do a=i;
  
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
    
  }
  st = getCurrentTime();
      ADR[D1] = BDR[D2];
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  
  }
 
  for (a,b) in (BDR[D2],ADR[D1]) do if (a!=b) then writeln("ERROR!!!!");
  
  // ==============================================================================
  
//  writeln("Default Rectangular <-- Default Rectangular. Example 2 :ADR",D1,"(L0) = BDR",D2,"(L0) on Locale 1");
  for (a,i) in (ADR,[1..n*n]) do a=i;
D1=[1..n,1..n];
D2=D1;
 on Locales(1)
 {
writeln();if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 2 :ADR",D1,"(",ADR.locale,") = BDR",D2,"(",BDR.locale,") on ", here.locale);  
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  st = getCurrentTime();
  
      ADR[D1] = BDR[D2];
  
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
 }
 
 for (a,b) in (BDR[D2],ADR[D1]) do if (a!=b) then writeln("ERROR!!!!");
  
 // ==============================================================================
//  D1=[1..n,n/4+1..3*n/4];
D2=D1;
var mes:string;
if(numLocales<3){
  writeln();if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 3: Not enough number of Locales ",numLocales," (3 needed)");
}
else{
  writeln();if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 3 :ADR",D1,"(L0) = BDR",D2,"(L2) on Locale 1");
  for (a,i) in (ADR,[1..n*n]) do a=i;

 on Locales(2)
 {
  
  var CDR: [1..n,1..n] real;
  for (a,i) in (CDR,[1..n*n]) do a=i+100;
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  
  st = getCurrentTime();
  
  on Locales(1){
      ADR[D1] = CDR[D2];
  }
  
  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
  for (a,b) in (CDR[D2],ADR[D1]) do if (a!=b) then writeln("ERROR!!!!");
 }
} 

// ==============================================================================


 writeln();if printOutput then  writeln("Default Rectangular <-- Default Rectangular. Example 4 :ADR",D1,"(L0) = CDR",D2,"(L1) on Locale 1"); 
  for (a,i) in (ADR,[1..n*n]) do a=i;

//for (a,i) in (CDR,[1..n*n]) do a=i+100;
 on Locales(1)
 {

  var CDR: [1..n,1..n] real;
  for (a,i) in (CDR,[1..n*n]) do a=i+100;
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }

  st = getCurrentTime();

//  on Locales(1){ 
      ADR[D1] = CDR[D2];
//  }

  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
  for (a,b) in (CDR[D2],ADR[D1]) do if (a!=b) then writeln("ERROR!!!!");
 }

// ==============================================================================

 writeln();if printOutput then  writeln("Default Rectangular <-- Default Rectangular. Example 5 :ADR",D1,"(L1) = BDR",D2,"(L0) on Locale 1"); 
  for (a,i) in (ADR,[1..n*n]) do a=i;

//var CCDR:[1..n,1..n] real;
 for (a,i) in (ADR,[1..n*n]) do a=i+100;
 on Locales(1)
 {

  var CDR: [1..n,1..n] real;
  for (a,i) in (CDR,[1..n*n]) do a=i;
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }

  st = getCurrentTime();

//  on Locales(1){ 
      CDR[D1] = ADR[D2];
//  }

  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
  for (a,b) in (CDR[D1],ADR[D2]) do if (a!=b) then writeln("ERROR!!!!");
 }
// ==============================================================================
  
var D3 = [1..10, 1..5];
//var D3 = [2..5, 2..5];
var H: [D3] real;
var G: [D3] real;
 writeln();if printOutput then  writeln("Default Rectangular <-- Default Rectangular. Example 6 :ADR",D3,"(L0) = CDR",D3,"(L1) on Locale 1");
 // for (a,i) in (ADR,[1..n*n]) do a=i;

/* on Locales(1)
 {
var G: [D3] real;
//  var CDR: [1..n,1..n] real;
  for (a,i) in (G,[1..n*n]) do a=i+100;
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }

  st = getCurrentTime();
*/
//  on Locales(1){
      //ADR[D3] = CDR[D3];
      //H[2..5 by 3,2..5] = CDR[D3];
      
      H[D3] = G[D3];
//  }

/*  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }*/
  for (a,b) in (G[D3],H[D3]) do if (a!=b) then writeln("ERROR!!!!");
// }
// ==============================================================================
//D1 = [1..n, 1..n by 2];
//D2=D1;

writeln();if printOutput then   writeln("Default Rectangular <-- Default Rectangular. Example 7 :ADR",D3,"(L1) = BDR",D3,"(L0) on Locale 1");
  for (a,i) in (ADR,[1..n*n]) do a=i;

//var CDR:[1..n,1..n] real;
 for (a,i) in (ADR,[1..n*n]) do a=i+100;
 on Locales(1)
 {

  var CDR: [1..n,1..n] real;
  for (a,i) in (CDR,[1..n*n]) do a=i;
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
  for (a,b) in (CDR[D3],ADR[D3]) do if (a!=b) then writeln("ERROR!!!!");
 }

// ==============================================================================

var D4 = [1..n by 2, 1..n by 2];
  writeln();if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 8 :ADR",D4,"(L0) = CDR",D4,"(L1) on Locale 1");
  for (a,i) in (ADR,[1..n*n]) do a=i;

 on Locales(1)
 {

  var CDR: [1..n,1..n] real;
  for (a,i) in (CDR,[1..n*n]) do a=i+100;
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
  for (a,b) in (CDR[D4],ADR[D4]) do if (a!=b) then writeln("ERROR!!!!");
 }

// ==============================================================================

 writeln();if printOutput then writeln("Default Rectangular <-- Default Rectangular. Example 9 :ADR",D4,"(L1) = BDR",D4,"(L0) on Locale 1");
  for (a,i) in (ADR,[1..n*n]) do a=i;

//var CDR:[1..n,1..n] real;
 for (a,i) in (ADR,[1..n*n]) do a=i+100;
 on Locales(1)
 {

  var CDR: [1..n,1..n] real;
  for (a,i) in (CDR,[1..n*n]) do a=i;
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
  for (a,b) in (CDR[D4],ADR[D4]) do if (a!=b) then writeln("ERROR!!!!");
 }

// ==============================================================================

var D5 = [1..n by 10, 1..n];

 writeln();if printOutput then  writeln("Default Rectangular <-- Default Rectangular. Example 10 :ADR",D5,"(L0) = CDR",D5,"(L1) on Locale 1");
  for (a,i) in (ADR,[1..n*n]) do a=i;

 on Locales(1)
 {

  var CDR: [1..n,1..n] real;
  for (a,i) in (CDR,[1..n*n]) do a=i+100;
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
  for (a,b) in (CDR[D5],ADR[D5]) do if (a!=b) then writeln("ERROR!!!!");
 }

// ==============================================================================

 writeln();if printOutput then  writeln("Default Rectangular <-- Default Rectangular. Example 11 :ADR",D5,"(L1) = BDR",D5,"(L0) on Locale 1");
  for (a,i) in (ADR,[1..n*n]) do a=i;

//var CDR:[1..n,1..n] real;
 for (a,i) in (ADR,[1..n*n]) do a=i+100;
 on Locales(1)
 {

  var CDR: [1..n,1..n] real;
  for (a,i) in (CDR,[1..n*n]) do a=i;
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
  for (a,b) in (CDR[D5],ADR[D5]) do if (a!=b) then writeln("ERROR!!!!");
 }
 
 // ==============================================================================

 writeln();if printOutput then  writeln("Default Rectangular <-- Default Rectangular. Example 12 :ADR",D5,"(L1) = BDR",D5,"(L0) on Locale 1");
  for (a,i) in (ADR,[1..n*n]) do a=i;

//var CDR:[1..n,1..n] real;
 for (a,i) in (ADR,[1..n*n]) do a=i+100;
 on Locales(1)
 {

  var CDR: [1..n,1..n] real;
  for (a,i) in (CDR,[1..n*n]) do a=i;
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }

  st = getCurrentTime();

//  on Locales(1){
      CDR[1..10,1..10] = ADR[1..10,1..10];
//  }

  dt = getCurrentTime()-st;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
    writeln("Time: ", dt);
  }
  for (a,b) in (CDR[1..10,1..10],ADR[1..10,1..10]) do if (a!=b) then writeln("ERROR!!!!");
 }

}
