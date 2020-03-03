use driver_domains;

var x1: Dom1D.type;
var x2: Dom2D.type;

test(x1);
test(x2);

proc compare(D, R, a, s=2) {
  writeln("Testing ", D, "[.. by ", s, " align ", a, "]");
  const ranges = rangeTuple(D.rank, .. by s align a);
  const dd = D[(...ranges)];
  const rr = R[(...ranges)];

  type yieldType = if rr.rank == 1 then rr.idxType else rr.rank*rr.idxType;
  var tracker : domain(yieldType);

  forall i in rr do tracker.add(i);
  assert(tracker.size == rr.size);

  {
    var failures : int;
    for i in dd do if !tracker.contains(i) then failures += 1;

    write("\tserial iter: ");
    if failures == 0 then writeln("SUCCESS");
    else writeln(failures, " FAILURES");
  }

  {
    var failures : atomic int;
    forall i in dd do if !tracker.contains(i) then failures.add(1);

    const f = failures.read();
    write("\tleader/follower: ");
    if f == 0 then writeln("SUCCESS");
    else writeln(f, " FAILURES");
  }
}

proc test(ref D) {
  D = rangeTuple(D.rank, 1..10);
  var R : domain(D.rank, D.idxType, D.stridable);
  R; R = D; // disable split init because warning text differs per dist

  compare(D, R, 0);
  compare(D, R, 1);
}

proc rangeTuple(param rank : int, rng) {
  var ret : rank*rng.type;
  for param i in 1..rank do ret(i) = rng;
  return ret;
}
