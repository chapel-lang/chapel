// This test illustrates the current default forall intent (see .good)
// and how that intent is not implemented properly (see .bad).
// It is crafted so that both the correct behavior and the current behavior
// are predictable.

config const sz = 4;
const RNG = 1..sz;

record MyRecord {
  var intField: int;
}

var  rGlob: MyRecord;
rGlob.intField = 111;
writeln("step 1  ", rGlob.intField);

forall jjj1 in myIter() do tester1(rGlob,jjj1);

writeln("step 4  ", rGlob.intField);

proc tester1(rArg1: MyRecord, idx1: int) {
  writeln("step 2:  ", rArg1.intField, "  ", rGlob.intField);
  // The first time this is executed, we should see rGlob and rArg1
  // both switch from 111 to 222. After that, it should all be 222s.
  rGlob.intField = 222;
  writeln("step 3:  ", rArg1.intField, "  ", rGlob.intField);
}

iter myIter() {
  for jjjX in RNG do
    yield jjjX;
}

iter myIter(param tag) where tag == iterKind.standalone {
  coforall task in 1..2 do
    // force task intents while keeping things serial
    if task == 1 then
      for jjjY in myIter() do
        yield jjjY;
}
