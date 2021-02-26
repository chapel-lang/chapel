use BlockDist;
use CyclicDist;

var a = newBlockArr(0..10, int);
var b = newBlockArr(0..10, int);
var c = newCyclicArr(0..10, int);

for i in b.domain {
  b[i] = i;
}

// a is a static candidate, b is dynamic. Both accesses will be replaced by
// localAccess, so no need to aggregate
// NOTE: this'll end up to be 2 foralls, where 1 of them won't have a
// localAccess for b (because it is a dynamic candidate). For that forall, we
// will generate aggregation, but that forall will never execute, so we expect
// NOT to see any verbose aggregation output from this loop
writeln("Beginning forall 1");
forall i in a.domain {
  a[i] = b[i];
}
writeln("Ending forall 1");

writeln(a);

// a is a static candidate, c is dynamic. RHS will remain as is, LHS will be
// local access, we should do source aggregation
writeln("Beginning forall 2");
forall i in a.domain {
  a[i] = c[i];
}
writeln("Ending forall 2");

writeln(a);
