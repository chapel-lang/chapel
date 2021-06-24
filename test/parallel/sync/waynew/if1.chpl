// Test a sync var in a conditional.

use Time;

param N = 20;
var a: [1..N] int;
var done: sync bool;


for i in 1..N/2 do
  a[i] = i;

begin {
  sleep( 2);
  for j in (N/2+1)..N do
    a[j] = N+j-(N/2);
  done.writeEF(true);
}

if done.readFE() then
  writeln( a);
