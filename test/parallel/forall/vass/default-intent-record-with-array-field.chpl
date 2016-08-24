// As of this writing, records are copied under the default forall intent.
// For an array field, the array reference happens to be copied, so it results
// in the correct behavior, which is currently 'const ref' in this case.

config const sz = 16;
const RNG = 1..sz;

record MyRecord {
  var arrField: [RNG] int;
}

var rGlob: MyRecord;
rGlob.arrField = 1..sz;
writeln("step 1:  ", rGlob.arrField);

forall jjj1 in RNG do tester1(rGlob,jjj1);

writeln("step 4:  ", rGlob.arrField);

proc tester1(rArg1: MyRecord, idx1: int) {
  writeln("step 2[", idx1, "]  ", rArg1.arrField[idx1]);
  rGlob.arrField[idx1] = idx1*10;
  writeln("step 3[", idx1, "]  ", rArg1.arrField[idx1]);
}
