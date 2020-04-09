use BlockDist;
use PrintComms;
config const doDiagnostics=false;

config  const n: int=50;
var Dist1 = new dmap(new Block({1..n,1..n}));
var Dist2 = new dmap(new Block({1..(2*n),1..(2*n)}));
var Dom1: domain(2,int) dmapped Dist1 = {1..n,1..n};
var Dom2: domain(2,int) dmapped Dist2 = {1..n,1..n};

var A:[Dom1] real; //int(64);
var B:[Dom2] real;

proc main(){

  var a,b:real;
  var i:int;

  for (a,i) in zip(A,{1..n*n}) do a=i;
  for (b,i) in zip(B,{1..n*n}) do b=i+100.0;
  /* writeln("Original vectors:"); */
  /* writeln("==================="); */
  /* writeln("A= ", A); */
  /* writeln("B= ", B); */

  //  writeln();
  writeln("Data movement with useBulkTransfer=",useBulkTransfer);
  if doDiagnostics then startCommDiagnostics();
  A=B;
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
  }
  writeln("===============");
  //writeln("Comms details WITH aggregation: ",getCommDiagnostics());
  for (a,b) in zip(A,B) do if (a!=b) then writeln("ERROR!!!!");
  //  writeln();

  //Reset array A
  for (a,i) in zip(A,{1..n*n}) do a=i;

  writeln("Data movement with Slicing and with useBulkTransfer=",useBulkTransfer);
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  A[1..n,1..n]=B[1..n,1..n];
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("");
  }
  writeln("===============");
  //writeln("Comms WITH aggregation and with Slicing: ",getCommDiagnostics());
  for (a,b) in zip(A[1..n,1..n],B[1..n,1..n]) do if (a!=b) then writeln("ERROR!!!!");
  //writeln();

}
