use ReplicatedDist;

var locD = {1..5};
var D = locD dmapped new replicatedDist();

var A: [D] int;

coforall loc in Locales with (ref A) do on loc {
  A = here.id;
}

for loc in Locales {
  writeln(A.replicand(loc));
}
