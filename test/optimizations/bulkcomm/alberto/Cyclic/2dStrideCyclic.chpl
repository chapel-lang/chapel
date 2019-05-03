use CyclicDist;
use PrintComms;
config const printOutput=true;
config const doDiagnostics=false;

config  const n: int=8;
var e=false;
//var Dist1 = new dmap(new Block({1..n,1..n}));
//var Dist2 = new dmap(new Block({1..(2*n),1..(2*n)}));
//var Dom1: domain(2,int) dmapped Dist1 = {1..n,1..n};
//var Dom2: domain(2,int) dmapped Dist2 = {1..2*n,1..2*n};
const Space={1..n,1..n};
const Dom1: domain(2) dmapped Cyclic(startIdx=Space.low)=Space;
const Space2={1..2*n,1..2*n};
const Dom2: domain(2) dmapped Cyclic(startIdx=Space2.low)=Space2;

var A:[Dom1] int(64); 
var B:[Dom2] int(64);
var C:[Space] real(64);
var D:[Space2] real(64);
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
  for (b,i) in zip(B,{1..2*2*n*n}) do b=i+100;

  if printOutput {
    writeln();
  }
  var DomA = Dom1[1..3 by 1,2..7 by 1];
  var DomB = Dom2[3..5 by 1,5..10 by 1];

  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(A[DomA]);
    writeln();
    writeln("B of Dom B Distribution: ");
    LocaleDistribution(B[DomB]);
    writeln();
  }

  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
   // writeln("Example A) A",DomA," <-- B",DomB);
  A[DomA]=B[DomB];
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("Example 1) ");
  }
  if printOutput {
    writeln("Example 1) A",DomA," <-- B",DomB);
    writeln("A= ",A);
    writeln("B= ",B);
  }
  for (a,b) in zip(A[DomA],B[DomB]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

  //EXAMPLE 2
  
  for (a,i) in zip(A,{1..n*n}) do a=i;
  for (b,i) in zip(B,{1..2*2*n*n}) do b=i+100;
  
   DomA = Dom1[1..5 by 2,1..3];
   DomB = Dom2[2..4,1..5 by 2];
    writeln("Example 2) A",DomA," <-- B",DomB);
  //DomA = Dom1[1..3,1..3];
  //DomB = Dom2[2..4,12..14];
  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(A[DomA]);
    writeln();
    writeln("B of Dom B Distribution: ");
    LocaleDistribution(B[DomB]);
    writeln();
  }

  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
 // C[DomA]=D[DomB];
  A[DomA]=B[DomB];
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("Example 2) ");
  }
  if printOutput {
    writeln("Example 2) A",DomA," <-- B",DomB);
    writeln("A= ",A);
    writeln("B= ",B);
  }
  for (a,b) in zip(A[DomA],B[DomB]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

//EXAMPLE 3
  for (a,i) in zip(A,{1..n*n}) do a=i;
  for (b,i) in zip(B,{1..2*2*n*n}) do b=i+100;
  
  DomA = Dom1[1..3,5..7];
  DomB = Dom2[6..8,1..3];
  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(A[DomA]);
    writeln();
    writeln("B of Dom B Distribution: ");
    LocaleDistribution(B[DomB]);
    writeln();
  }

  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  //  writeln("Example 3) A",DomA," <-- B",DomB);
  A[DomA]=B[DomB];
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("Example 3) ");
  }
  if printOutput {
    writeln("Example 3) A",DomA," <-- B",DomB);
    writeln("A= ",A);
    writeln("B= ",B);
  }
  for (a,b) in zip(A[DomA],B[DomB]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }
  
  //EXAMPLE 4
  for (a,i) in zip(A,{1..n*n}) do a=i;
  for (b,i) in zip(B,{1..2*2*n*n}) do b=i+100;
  
  DomA = Dom1[1..3,4..6];
  DomB = Dom2[6..8,3..5];
  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(A[DomA]);
    writeln();
    writeln("B of Dom B Distribution: ");
    LocaleDistribution(B[DomB]);
    writeln();
  }

  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  //  writeln("Example 4) A",DomA," <-- B",DomB);
  A[DomA]=B[DomB];
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("Example 4) ");
  }
  if printOutput {
    writeln("Example 4) A",DomA," <-- B",DomB);
    writeln("A= ",A);
    writeln("B= ",B);
  }
  for (a,b) in zip(A[DomA],B[DomB]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

 //EXAMPLE 5
  for (a,i) in zip(A,{1..n*n}) do a=i;
  for (b,i) in zip(B,{1..2*2*n*n}) do b=i+100;
  
  DomA = Dom1[4..5,4..5];
    DomB = Dom2[5..6,4..5];
  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(A[DomA]);
    writeln();
    writeln("B of Dom B Distribution: ");
    LocaleDistribution(B[DomB]);
    writeln();
  }

  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
 //   writeln("Example 5) A",DomA," <-- B",DomB);
  A[DomA]=B[DomB];
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("Example 5) ");
  }
  if printOutput {
    writeln("Example 5) A",DomA," <-- B",DomB);
    writeln("A= ",A);
    writeln("B= ",B);
  }
  for (a,b) in zip(A[DomA],B[DomB]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }
   
  //EXAMPLE 6
  for (a,i) in zip(A,{1..n*n}) do a=i;
  for (b,i) in zip(B,{1..2*2*n*n}) do b=i+100;
  
  DomA = Dom1[1..n-1,1..n-1];
  DomB = Dom2[2..n,2..n];
  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(A[DomA]);
    writeln();
    writeln("B of Dom B Distribution: ");
    LocaleDistribution(B[DomB]);
    writeln();
  }

  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
 //   writeln("Example 6) A",DomA," <-- B",DomB);
  A[DomA]=B[DomB];
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("Example 6) ");
  }
  if printOutput {
    writeln("Example 6) A",DomA," <-- B",DomB);
    writeln("A= ",A);
    writeln("B= ",B);
  }
  for (a,b) in zip(A[DomA],B[DomB]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }
  
  //EXAMPLE 7
  for (a,i) in zip(A,{1..n*n}) do a=i;
  for (b,i) in zip(B,{1..2*2*n*n}) do b=i+100;
  
  DomA = Dom1[1..n,1..n];
  DomB = Dom2[1..n,1..n];
  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(A[DomA]);
    writeln();
    writeln("B of Dom B Distribution: ");
    LocaleDistribution(B[DomB]);
    writeln();
  }

  if doDiagnostics {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
  //  writeln("Example 7) A",DomA," <-- B",DomB);
  A[DomA]=B[DomB]; 
  if doDiagnostics {
    stopCommDiagnostics();
    myPrintComms("Example 7) ");
    }
  if printOutput {
    writeln("Example 7) A",DomA," <-- B",DomB);
    writeln("A= ",A);
    writeln("B= ",B);
  }
  for (a,b) in zip(A[DomA],B[DomB]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }
  
  if e then writeln("Hey man!!!!... double check it! There was an ERROR!");
  proc LocaleDistribution(X:[]){
    var BA : [X.domain] int;
    forall ba in BA do
      ba = here.id;
    writeln(BA);
  }
}
