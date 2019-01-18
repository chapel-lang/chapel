// Test to check the correctness of the dynamic() Iterator from the DynamicIters module
use DynamicIters;

config const nTasks=4;          // number of cores; should be here.maxTaskPar?
config const n:int=10000;       // The size of the range
config const chunkSize:int=100; // The size of the chunk
var rng:range=1..n;             // The ranges
var rngs=rng by 2;
var dmn:domain(1)={rng};       // The domains
var dmns=dmn by 2;

var A:[rng] int=0;            // The test arrays
var B:[rngs] int=0;
var C:[rngs,rng] int=0;
var D:[dmn] int=0;
var E:[dmns] int=0;
var F:[{rngs,rng}] int=0;

writeln("Checking a non-strided range");
// The iterator
forall i in dynamic(rng,chunkSize,nTasks) do {
  A[i]=A[i]+1;
}

// Check if parallel assignment of Arr[] using dynamic() Iterator is correct
checkCorrectness(A,rng);

writeln("Checking a non-strided domain");
// The iterator
forall i in dynamic(dmn,chunkSize,nTasks) do {
  D[i]=D[i]+1;
}

// Check if parallel assignment of Arr[] using dynamic() Iterator is correct
checkCorrectness(D,dmn);

writeln("Checking a strided range");
// The iterator
forall i in dynamic(rngs,chunkSize,nTasks) do {
  B[i]=B[i]+1;
}

// Check if parallel assignment of Arr[] using dynamic() Iterator is correct
checkCorrectness(D,dmn);

writeln("Checking a strided domain");
// The iterator
forall i in dynamic(dmns,chunkSize,nTasks) do {
  E[i]=E[i]+1;
}

// Check if parallel assignment of Arr[] using dynamic() Iterator is correct
checkCorrectness(E,dmns);

writeln("Checking a zippered iteration (range)");
// The iterator
forall (i,j) in zip(dynamic(rngs,chunkSize,nTasks),rng#rngs.size) do {
  C[i,j]=C[i,j]+1;
}

// Check if parallel assignment of Arr[] using dynamic() Iterator is correct
checkCorrectness2(C,rngs,rng);

writeln("Checking a zippered iteration (domain)");
// The iterator
forall (i,j) in zip(dynamic(dmns,chunkSize,nTasks),dmn#dmns.size) do {
  F[i,j]=F[i,j]+1;
}

// Check if parallel assignment of Arr[] using dynamic() Iterator is correct
checkCorrectness2(F,dmns,dmn);

proc checkCorrectness(Arr:[]int,r:range(?))
{
  var check=true;
  for i in r do {
    if Arr[i] != 1 then {
      check=false;
      writeln(" ");
      writeln("Dynamic Iterator: Error in iteration ", i);
      writeln(" ");
    }
  }
  if check==true then
    writeln("Dynamic Iterator: Correct");
}

proc checkCorrectness(Arr:[]int,c:domain)
{
  var check=true;
  for i in c do {
    if Arr[i] != 1 then {
      check=false;
      writeln(" ");
      writeln("Dynamic Iterator: Error in iteration ", i);
      writeln(" ");
    }
  }
  if check==true then
    writeln("Dynamic Iterator: Correct");
}

proc checkCorrectness2(Arr:[]int,r:range(?), r2:range(?))
{
  var check=true;
  for (i,j) in zip(r,r2#r.size) do {
    if Arr[i,j] != 1 then {
      check=false;
      writeln(" ");
      writeln("Dynamic Iterator: Error in iteration ", i, ",",j);
      writeln(" ");
    }
  }

  if check==true then
    writeln("Dynamic Iterator: Correct");

}

proc checkCorrectness2(Arr:[]int,c:domain, c2:domain)
{
  var check=true;
  for (i,j) in zip(c,c2#c.size) do {
    if Arr[i,j] != 1 then {
      check=false;
      writeln(" ");
      writeln("Dynamic Iterator: Error in iteration ", i, ",",j);
      writeln(" ");
    }
  }

  if check==true then
    writeln("Dynamic Iterator: Correct");

}
