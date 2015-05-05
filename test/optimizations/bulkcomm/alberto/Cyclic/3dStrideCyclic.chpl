use CyclicDist;
use PrintComms;
config const printOutput=false;
config const doDiagnostics=false;

config const m = 8;//6;
config const n = 8;//3;
config const o = 8;//4;
var e=false;
//var Dist1 = new dmap(new Block({1..m,1..n,1..o}));
//var Dom: domain(3,int) dmapped Dist1 = {1..m,1..n,1..o};
const S = {1..m,1..n,1..o};
const Dom: domain(3) dmapped Cyclic(startIdx=S.low)=S;

var A:[Dom] int(64); //real
var B:[Dom] int(64);

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
  for (a,i) in zip(A,{1..m*n*o}) do a=i;
  for (b,i) in zip(B,{1..m*n*o}) do b=i+100;

  if printOutput {
    writeln();
  }
  var Dom2 = {1..1,1..5 by 2,1..3};
  var Dom1 = {2..2,2..4,1..5 by 2};
    
  //var Dom1={1..m,1..n,1..o};
  //var Dom2={1..m,1..n,1..o};
  //Dom1=Dom2;

  //EXAMPLE 1

  A[Dom2]=B[Dom1];

  for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

  if printOutput
    {
      writeln("Example 1: A",Dom2," = B",Dom1);
      for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
      for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
    }
    
  //EXAMPLE 2

  Dom2={1..2,3..4,4..4};
  Dom1={3..4,2..3,3..3};

  for (a,i) in zip(A,{1..m*n*o}) do a = i;
  for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

  A[Dom2]=B[Dom1];

  for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

  if printOutput
    {
      writeln("Example 2: A",Dom2," = B",Dom1);
      for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
      for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
    }


  //EXAMPLE 3
  for (a,i) in zip(A,{1..m*n*o}) do a = i;
  for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

  Dom2={1..2,3..3,3..4};
  Dom1={2..3,1..1,2..3};

  A[Dom2]=B[Dom1];

  for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

  if printOutput
    {
      writeln("Example 3: A",Dom2," = B",Dom1);
      for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
      for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
    }

  //EXAMPLE 4
  for (a,i) in zip(A,{1..m*n*o}) do a = i;
  for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

  Dom2={1..2,1..4,4..4};
  Dom1={3..4,1..4,1..1};
  A[Dom2]=B[Dom1];

  for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

  if printOutput
    {
      writeln("Example 4: A",Dom2," = B",Dom1);
      for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
      for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
    }

  //EXAMPLE 5
  for (a,i) in zip(A,{1..m*n*o}) do a = i;
  for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

  Dom2={1..2,3..3,1..4};
  Dom1={3..4,3..3,1..4};

  A[Dom2]=B[Dom1];

  for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

  if printOutput
    {
      writeln("Example 5: A",Dom2," = B",Dom1);
      for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
      for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
    }

  //EXAMPLE 6
  for (a,i) in zip(A,{1..m*n*o}) do a = i;
  for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

  Dom2={1..2,1..4,3..4};
  Dom1={3..4,1..4,2..3};

  A[Dom2]=B[Dom1];

  for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

  if printOutput
    {
      writeln("Example 6: A",Dom2," = B",Dom1);
      for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
      for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
    }

  //EXAMPLE 7
  for (a,i) in zip(A,{1..m*n*o}) do a = i;
  for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

  Dom2={1..2,3..4,1..4};
  Dom1=Dom2;

  A[Dom2]=B[Dom1];

  for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

  if printOutput
    {
      writeln("Example 7: A",Dom2," = B",Dom1);
      for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
      for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
    }

  //EXAMPLE 8
  for (a,i) in zip(A,{1..m*n*o}) do a = i;
  for (b,i) in zip(B,{1..m*n*o}) do b = i+500;

  Dom2={1..2,3..4,2..4};
  Dom1=Dom2;

  A[Dom2]=B[Dom1];

  for (a,b) in zip(A[Dom2],B[Dom1]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};

  if printOutput
    {
      writeln("Example 8: A",Dom2," = B",Dom1);
      for (i,j,k) in Dom do writeln("A[",i,",", j,",",k,"] = ",A[i,j,k]);
      for (i,j,k) in Dom do writeln("B[",i,",", j,",",k,"] = ",B[i,j,k]);
    }

  if e then writeln("Hey man!!!!... double check it! There was an ERROR!");

  proc LocaleDistribution(X:[]){
    var BA : [X.domain] int;
    forall ba in BA do
      ba = here.id;
    writeln(BA);
  }
writeln();  

}
