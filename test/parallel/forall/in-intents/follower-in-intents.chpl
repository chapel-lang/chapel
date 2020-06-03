
/// iterator ///

iter myIter(in INARG) {
  yield 77;
}

iter myIter(in INARG, param tag) where tag==iterKind.leader
{
  writeln("leader ", INARG);
  yield update(INARG, 100);
  yield update(INARG, 200);
}

iter myIter(in INARG, param tag, followThis) where tag==iterKind.follower
{
  writeln("follower ", INARG, "   following ", followThis);
  yield update(INARG, 2);
  yield update(INARG, 5);
}

/// integer ///

var globalInt = 1000;
proc update(ref num: int, inc:int) { num += inc; return num; }

writeln(); writeln("int");
forall iidx in zip(myIter(globalInt)) do
  writeln(iidx);
writeln(globalInt);

writeln(); writeln("int x2");
forall iidx in zip(myIter(globalInt),myIter(globalInt)) do
  writeln(iidx);
writeln(globalInt);

/// record ///

record RR { var xx: int; }
var globalRec = new RR(1000);
proc update(ref rr: RR, inc:int) { rr.xx += inc; return rr.xx; }

writeln(); writeln("RR");
forall ridx in zip(myIter(globalRec)) do
  writeln(ridx);
writeln(globalRec);

writeln(); writeln("RR x2");
forall ridx in zip(myIter(globalRec),myIter(globalRec)) do
  writeln(ridx);
writeln(globalRec);

/// done ///

writeln();
