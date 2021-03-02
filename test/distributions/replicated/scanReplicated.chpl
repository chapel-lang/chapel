use ReplicatedDist;

var D = {1..5} dmapped Replicated();
var A: [D] int;

for loc in Locales do
  on loc do
    A = here.id+1;

for loc in Locales do
  on loc do
    writeln(A.replicand(loc));

var B = + scan A;

writeln(B.type:string);

for loc in Locales do
  on loc do
    writeln(B.replicand(loc));

A = here.id + 1;

var C: [LocaleSpace][1..5] int;

for loc in Locales do
  on loc do
    C[here.id] = + scan A;

writeln(C);
