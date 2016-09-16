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

// another way to run a forall loop
const redResult = + reduce [jjj2 in RNG] tester2(rGlob,jjj2);

writeln("step 7:  ", rGlob.arrField);
assert(redResult == sz);

proc tester1(rArg1: MyRecord, idx1: int) {
  writeln("step 2[", idx1, "]  ", rArg1.arrField[idx1]);
  rGlob.arrField[idx1] = idx1*10;
  writeln("step 3[", idx1, "]  ", rArg1.arrField[idx1]);
}

proc tester2(rArg2: MyRecord, idx2: int) {
  writeln("step 5[", idx2, "]  ", rArg2.arrField[idx2]);
  rGlob.arrField[idx2] = idx2*100;
  writeln("step 6[", idx2, "]  ", rArg2.arrField[idx2]);
  return 1;
}
