class Gen {
  type t;
  var x: t;
}

class NotGen {
  var x: int;
}

config param useGeneric = false;
config param useSyncFormal = true;
type FormalType = if useGeneric then Gen(?) else NotGen;
type ActualType = if useGeneric then owned Gen(int) else owned NotGen;

// The essence of this test is to serve as a regression test for a bug
// caused by the 'canInstantiate' check in the compiler. This incorrectly
// returned false for ('anymanaged Gen' <- 'owned Gen(int)'), whereas
// ('anymanaged NotGen' <- 'owned NotGen') was correctly allowed.
//
// The only way get trigger the bug through function calls is by using
// the special logic for syncs / singles when computing the instantiation type.
// To make this work, this PR makes use of a deprecated implicit read for syncs.
// When the deprecation goes through and the implicit read is removed, this test
// will stop working, and there will be no way to detect any difference in behavior
// between the buggy and non-buggy 'canInstantiate'. This test should be removed
// then.
compilerWarning("----------------------------------------------------------------");
compilerWarning("Delete this test when implicit sync reads are no longer allowed.");
compilerWarning("----------------------------------------------------------------");

pragma "last resort"
proc test(x) { compilerWarning("Fell through"); }

proc test(myvalue: FormalType) where useSyncFormal { compilerWarning("Accepted"); }
proc test(myvalue: sync FormalType) where !useSyncFormal { compilerWarning("Accepted"); }

var value: sync ActualType;
test(value);
