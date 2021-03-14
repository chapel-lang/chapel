use BlockDist;
use PrintComms;
config const printOutput=false;
config const doDiagnostics=false;

config  const n: int=4;
var e=false;
var Dist1 = new dmap(new Block({1..n,1..n}));
var Dom1: domain(2,int) dmapped Dist1 = {1..n,1..n};

var A:[Dom1] int(64); //real
var B:[Dom1] int(64);
proc main(){
  var ADR: [1..n,1..n,1..n] real;
  var BDR: [1..n,1..n,1..n] real;

  var a,b:real;
  var i:int;
  for (a,i) in zip(ADR,{1..n*n*n}) do a=i;
  for (b,i) in zip(BDR,{1..n*n*n}) do b=i+100;
  if printOutput {
    writeln();
  }

  var DomA = {1..2 by 1,1..3 by 1,2..4 by 4};
  var DomB = {1..2 by 1,1..3 by 1,2..4 by 4};
  if printOutput then
    writeln("Example 1 ADR",DomA," = BDR",DomB);
  ADR[DomA]=BDR[DomB];
  for (a,b) in zip(BDR[DomB],ADR[DomA]) do if (a!=b) then writeln("ERROR!!!!");
  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(ADR[DomA]);
    writeln();
  }
  
  for (a,i) in zip(ADR,{1..n*n*n}) do a=i;
  for (b,i) in zip(BDR,{1..n*n*n}) do b=i+100;

  //  ejemplo 2
 DomA = {1..2 by 1,1..2 by 1,2..4 by 2};
 DomB = {1..2 by 1,1..2 by 1,2..4 by 2};
  if printOutput then
    writeln("Example 2 ADR",DomA," = BDR",DomB);
  ADR[DomA]=BDR[DomB];
  for (a,b) in zip(BDR[DomB],ADR[DomA]) do if (a!=b) then writeln("ERROR!!!!");
  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(ADR[DomA]);
    writeln();
  }
 
  for (a,i) in zip(ADR,{1..n*n*n}) do a=i;
  for (b,i) in zip(BDR,{1..n*n*n}) do b=i+100;

  //  ejemplo 3
 DomA = {1..2 by 1,1..4 by 2,2..4 by 2};
 DomB = {1..2 by 1,1..4 by 2,2..4 by 2};
   if printOutput then
    writeln("Example 3 ADR",DomA," = BDR",DomB);
  ADR[DomA]=BDR[DomB];
  for (a,b) in zip(BDR[DomB],ADR[DomA]) do if (a!=b) then writeln("ERROR!!!!");
  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(ADR[DomA]);
    writeln();
  }
  
  for (a,i) in zip(ADR,{1..n*n*n}) do a=i;
  for (b,i) in zip(BDR,{1..n*n*n}) do b=i+100;

  //  ejemplo 4
 DomA = {1..2 by 1,1..4 by 3,2..4 by 2};
 DomB = {1..2 by 1,1..4 by 3,2..4 by 2};
  if printOutput then
    writeln("Example 4 ADR",DomA," = BDR",DomB);
  ADR[DomA]=BDR[DomB];
  for (a,b) in zip(BDR[DomB],ADR[DomA]) do if (a!=b) then writeln("ERROR!!!!");
  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(ADR[DomA]);
    writeln();
  }
 
  for (a,i) in zip(ADR,{1..n*n*n}) do a=i;
  for (b,i) in zip(BDR,{1..n*n*n}) do b=i+100;

  //  ejemplo 5
  DomA = {1..3 by 2,1..4 by 2,2..4 by 2};
  DomB = {1..3 by 2,1..4 by 2,2..4 by 2};
  if printOutput then
    writeln("Example 5 ADR",DomA," = BDR",DomB);
  ADR[DomA]=BDR[DomB];
  for (a,b) in zip(BDR[DomB],ADR[DomA]) do if (a!=b) then writeln("ERROR!!!!");
  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(ADR[DomA]);
    writeln();
  }

  for (a,i) in zip(ADR,{1..n*n*n}) do a=i;
  for (b,i) in zip(BDR,{1..n*n*n}) do b=i+100;

 //ejemplo 6
   DomA = {1..3 by 2,1..4 by 4,2..4 by 2};
   DomB = {1..3 by 2,1..4 by 4,2..4 by 2};
  if printOutput then
    writeln("Example 6 ADR",DomA," = BDR",DomB);
  ADR[DomA]=BDR[DomB];
    
  for (a,b) in zip(BDR[DomB],ADR[DomA]) do if (a!=b) then writeln("ERROR!!!!");
  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(ADR[DomA]);
    writeln();
  }
 
  for (a,i) in zip(ADR,{1..n*n*n}) do a=i;
  for (b,i) in zip(BDR,{1..n*n*n}) do b=i+100;

 //ejemplo 7
  DomA = {1..3 by 2,1..1 by 1,2..4 by 2};
  DomB = {1..3 by 2,1..1 by 1,2..4 by 2};
  if printOutput then
    writeln("Example 7 ADR",DomA," = BDR",DomB);
  ADR[DomA]=BDR[DomB];
  for (a,b) in zip(BDR[DomB],ADR[DomA]) do if (a!=b) then writeln("ERROR!!!!");
  if printOutput {
    writeln("A of DomA Distribution: ");
    LocaleDistribution(ADR[DomA]);
    writeln();
  }

  if printOutput {
    writeln("B of Dom B Distribution: ");
    LocaleDistribution(BDR[DomB]);
    writeln();
  }

  if e then writeln("Hey man!!!!... double check it! There was an ERROR!");
writeln("");
  proc LocaleDistribution(X:[]){
    var BA : [X.domain] int;
    forall ba in BA do
      ba = here.id;
    writeln(BA);
  }
}
