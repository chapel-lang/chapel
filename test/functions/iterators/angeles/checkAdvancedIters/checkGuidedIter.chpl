// Test to check the correctness of the guided() Iterator from the AdvancedIters module
use AdvancedIters;

config const nTasks=4;       //here.numCores; should be here.maxTaskPar?
config const n:int=1000;     // The size of the range
var rng:range=1..n;          // The ranges
var rngs=rng by 2;

var A:[rng] int=0;           // The test arrays
var B:[rngs] int=0;
var C:[rngs,rng] int=0;

writeln("Checking a non-strided range ");
// The iterator 
forall i in guided(rng,nTasks) do {
  A[i]=A[i]+1;
 }
// Check if parallel assignment of Arr[] using guided() Iterator is correct

checkCorrectness(A,rng);


writeln("Checking a strided range ");
// The iterator 
forall i in guided(rngs,nTasks) do {
  B[i]=B[i]+1;
 }
// Check if parallel assignment of Arr[] using guided() Iterator is correct

checkCorrectness(B,rngs);

writeln("Checking a zippered iteration ");
// The iterator 
forall (i,j) in zip(guided(rngs,nTasks),rng#rngs.size) do {
  C[i,j]=C[i,j]+1;
 }

// Check if parallel assignment of Arr[] using dynamic() Iterator is correct
checkCorrectness2(C,rngs,rng);

proc checkCorrectness(Arr:[]int,r:range(?))
{
  var check=true;
  for i in r do {
    if Arr[i] != 1 then {
      check=false;
      writeln(" ");
      writeln("Guided Iterator: Error in iteration ", i);
      writeln(" ");
    }
  }
  if check==true then 
    writeln("Guided Iterator: Correct");
}

proc checkCorrectness2(Arr:[]int,r:range(?), r2:range(?))
{
  var check=true;
  for (i,j) in zip(r,r2#r.size) do {
    if Arr[i,j] != 1 then {
      check=false;
      writeln(" ");
      writeln("Guided Iterator: Error in iteration ", i, ",",j);
      writeln(" ");
    }
  }

  if check==true then 
    writeln("Guided Iterator: Correct");

}
