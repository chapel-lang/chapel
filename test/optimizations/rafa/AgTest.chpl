use BlockDist;
use PrintComms;
config const printOutput=false;
config const doDiagnostics=false;

config  const n: int=10;
var e=false;
var Dist1 = new dmap(new Block({1..n}));
var Dist2 = new dmap(new Block({1..(2*n)}));
var Dom1: domain(1,int) dmapped Dist1 = {1..n};
var Dom2: domain(1,int) dmapped Dist2 = {1..2*n};

var A:[Dom1] int(64); //real
var B:[Dom2] int(64);
proc main(){

  /* write("A Distribution: "); */
  /* LocaleDistribution(A,Dom1); */
  /* writeln(); */

  /* write("B Distribution: ");   */
  /* LocaleDistribution(B,Dom2); */
  /* writeln(); */

  var a,b:real;
  var i:int;
  for (a,i) in zip(A,{1..n}) do a=i;
  for (b,i) in zip(B,{1..2*n}) do b=i+100;
  /* writeln("Original vectors:"); */
  /* writeln("==================="); */
  /* writeln("A= ", A); */
  /* writeln("B= ", B); */

  if printOutput {
    writeln();
  }
  var DomA = {2..6};
  var DomB = {5..9};
  if doDiagnostics {
    //startVerboseComm();
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  A[DomA]=B[DomB];
  if doDiagnostics {
    stopCommDiagnostics();
    //stopVerboseComm();
    myPrintComms("Example A) ");
    //writeln("Case A) ",getCommDiagnostics());
  }
  if printOutput {
    writeln("Example A) A",DomA," <-- B",DomB);
    //writeln("A= ",A);
    //writeln("B= ",B);
  }
  for (a,b) in zip(A[DomA],B[DomB]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

  for (a,i) in zip(A,{1..n}) do a=i;
  var DomC = {3..5};
  var DomD = {8..10};
  if doDiagnostics {
    //startVerboseComm();
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  A[DomC]=B[DomD];
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("Example B) ");
  }
  if printOutput {
    writeln("Example B) A",DomC," <-- B",DomD);
    //writeln("A= ",A);
    //writeln("B= ",B);
  }
  for (a,b) in zip(A[DomC],B[DomD]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

  for (a,i) in zip(A,{1..n}) do a=i;
  var DomE = {2..5};
  var DomF = {9..12};
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  A[DomE]=B[DomF];
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("Example C) ");
    //writeln("Case C) ",getCommDiagnostics());
  }
  if printOutput {
    writeln("Example C) A",DomE," <-- B",DomF);
  }
  //writeln("A= ",A);
  //writeln("B= ",B);
  for (a,b) in zip(A[DomE],B[DomF]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

  for (a,i) in zip(A,{1..n}) do a=i;
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  B[1..4] = A[3..6];
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("Example D) ");
    //writeln("Case D) ",getCommDiagnostics());
  }
  if printOutput {
    writeln("Example D) B",{1..4}," <-- A",{3..6});
  }
  // writeln("A= ",A);
  //writeln("B= ",B);
  for (a,b) in zip(B[1..4],A[3..6]) do if (a!=b) {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

  for (a,i) in zip(A,{1..n}) do a=i;
  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  B[7..10]=A[3..6];
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("Example E) ");
    //writeln("Case E) ",getCommDiagnostics());
  }
  if printOutput {
    writeln("Example E) B",{7..10}," <-- A",{3..6});
    // writeln("A= ",A);
    //writeln("B= ",B);
  }
  for (a,b) in zip(B[7..10],A[3..6]) do if (a!=b) {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

  if e then writeln("Hey man!!!!... double check it! There was an ERROR!");

  proc LocaleDistribution(X:[] , Dom_: domain){
    for i in Dom_ do{
      write(X[i].locale," ");
    }
  }
}
