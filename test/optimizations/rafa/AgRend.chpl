use BlockDist;
use PrintComms;
config const printOutput=false;
config const doDiagnostics=false;

config  const n: int=10000;
var Dist1 = new dmap(new Block({1..n}));
var Dist2 = new dmap(new Block({1..2*n}));
var Dom1: domain(1,int) dmapped Dist1 = {1..n};
var Dom2: domain(1,int) dmapped Dist2 = {1..n};
var A:[Dom1] real; //int(64);
var B:[Dom2] real;

proc main(){

  var a,b:real;
  var i:int;

  for (a,i) in zip(A,{1..n}) do a=i;
  for (b,i) in zip(B,{1..n}) do b=i+100.0;
  /* writeln("Original vectors:"); */
  /* writeln("==================="); */
  /* writeln("A= ", A); */
  /* writeln("B= ", B); */

  writeln("Data movement useBulkTransfer=",useBulkTransfer);
  if doDiagnostics then startCommDiagnostics();
  A=B;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
  }
  writeln("===============");
  for (a,b) in zip(A,B) do if (a!=b) then writeln("ERROR!!!!");

  //Reset array A
  for (a,i) in zip(A,{1..n}) do a=i;

  writeln("Data movement with Slicing and useBulkTransfer=",useBulkTransfer);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  A[1..n]=B[1..n];
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
  }
  writeln("===============");


}
