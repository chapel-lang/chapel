/*
  Test to check the correctness of the distributedGuided() iterator from the
  DistributedIters module.
*/
use DistributedIters;

var rng:range=1..10;
var A:[rng] int=0;

/*
  Input must be a range, domain, or array, so this should cause a compiler
  error.
*/
writeln("Checking a void...");
var v:nothing;
for i in distributedGuided(v)
do A[i] = A[i]+1;
