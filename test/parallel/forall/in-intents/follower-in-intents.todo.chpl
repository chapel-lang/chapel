
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

/// string ///

var globalString = "1000";

proc atoi(arg: string) { var result=0; for cp in arg.codepoints() do
                           result = result*10+(cp-48); return result; }

proc update(ref str: string, inc:int) { str += inc:string; return str; }

writeln(); writeln("string");
forall sidx in zip(myIter(globalString)) do
  writeln(sidx);
writeln(globalString);

/// done ///

writeln();
