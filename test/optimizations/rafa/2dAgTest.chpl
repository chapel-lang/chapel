use BlockDist;
use PrintComms;
config const printOutput=false;
config const doDiagnostics=false;

config  const n: int=10;
var e=false;
var Dist1 = new dmap(new Block({1..n,1..n}));
var Dist2 = new dmap(new Block({1..(2*n),1..(2*n)}));
var Dom1: domain(2,int) dmapped Dist1 = {1..n,1..n};
var Dom2: domain(2,int) dmapped Dist2 = {1..2*n,1..2*n};
var A:[Dom1] int(64); //real
var B:[Dom2] int(64);
proc main(){

  if printOutput {
    writeln("A Distribution: ");
    LocaleDistribution(A);
    writeln();

    writeln("B Distribution: ");
    LocaleDistribution(B);
    writeln();
  }

  var a,b:real;
  var i:int;
  for (a,i) in zip(A,{1..n*n}) do a=i;
  for (b,i) in zip(B,{1..4*n*n}) do b=i+100;
  /* writeln("Original vectors:"); */
  /* writeln("==================="); */
  /* writeln("A= ", A); */
  /* writeln("B= ", B); */

  if printOutput {
    writeln();
  }
  var DomA = {5..8,2..6};
  var DomB = {2..5,5..9};

  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(A[DomA]);
    writeln();
  }

  if printOutput {
    writeln("B of Dom B Distribution: ");
    LocaleDistribution(B[DomB]);
    writeln();
  }

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
    writeln("A= ",A);
    writeln("B= ",B);
  }
  for (a,b) in zip(A[DomA],B[DomB]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

  /* for (a,i) in (A,{1..n}) do a=i; */
  /* var DomC = {3..5}; */
  /* var DomD = {8..10}; */
  /* //startVerboseComm(); */
  /* resetCommDiagnostics(); */
  /* startCommDiagnostics(); */
  /* A[DomC]=B[DomD]; */
  /* stopCommDiagnostics(); */
  /* myPrintComms("Example B) "); */
  /* writeln("Example B) A",DomC," <-- B",DomD); */
  /* //writeln("A= ",A); */
  /* //writeln("B= ",B); */
  /* for (a,b) in (A[DomC],B[DomD]) do if (a!=b) then {writeln("ERROR!!!!");e=true;}; */
  /* writeln(); */

  /* for (a,i) in (A,{1..n}) do a=i; */
  /* var DomE = {2..5}; */
  /* var DomF = {9..12}; */
  /* resetCommDiagnostics(); */
  /* startCommDiagnostics(); */
  /* A[DomE], B[DomF]; */
  /* stopCommDiagnostics(); */
  /* myPrintComms("Example C) "); */
  /* //writeln("Case C) ",getCommDiagnostics()); */
  /* writeln("Example C) A",DomE," <-- B",DomF); */
  /* //writeln("A= ",A); */
  /* //writeln("B= ",B); */
  /* for (a,b) in (A[DomE],B[DomF]) do if (a!=b) then {writeln("ERROR!!!!");e=true;}; */
  /* writeln(); */

  /* for (a,i) in (A,{1..n}) do a=i; */
  /* resetCommDiagnostics(); */
  /* startCommDiagnostics(); */
  /* B[1..4]=A[3..6]; */
  /* stopCommDiagnostics(); */
  /* myPrintComms("Example D) "); */
  /* //writeln("Case D) ",getCommDiagnostics()); */
  /* writeln("Example D) B",{1..4}," <-- A",{3..6}); */
  /* // writeln("A= ",A); */
  /* //writeln("B= ",B); */
  /* for (a,b) in (B[1..4],A[3..6]) do if (a!=b) {writeln("ERROR!!!!");e=true;}; */
  /* writeln(); */

  /* for (a,i) in (A,{1..n}) do a=i; */
  /* resetCommDiagnostics(); */
  /* startCommDiagnostics(); */
  /* B[7..10]=A[3..6]; */
  /* stopCommDiagnostics(); */
  /* myPrintComms("Example E) "); */
  /* //writeln("Case E) ",getCommDiagnostics()); */
  /* writeln("Example E) B",{7..10}," <-- A",{3..6}); */
  /* // writeln("A= ",A); */
  /* //writeln("B= ",B); */
  /* for (a,b) in (B[7..10],A[3..6]) do if (a!=b) {writeln("ERROR!!!!");e=true;}; */
  /* writeln(); */

  if e then writeln("Hey man!!!!... double check it! There was an ERROR!");

  proc LocaleDistribution(X:[]){
    var BA : [X.domain] int;
    forall ba in BA do
      ba = here.id;
    writeln(BA);
  }
}
