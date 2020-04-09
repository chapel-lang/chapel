use BlockDist;

var D = {1..10, 1..10} dmapped Block({1..10, 1..10});
var SD: sparse subdomain(D);
var A: [D] real;

use ReplicatedDist;

var DR = {1..10, 1..10} dmapped Replicated();
var AR: [DR] int;

coforall loc in Locales do
  on loc do
    AR = here.id + 1;

for i in 1..10 {
  SD += (i,i);
  SD += (10-i+1,i);
}

A[SD] = AR[SD];

writeln(A);
