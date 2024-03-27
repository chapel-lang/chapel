module Unused {
  proc myProc(A, B) {
    for i in 1..10 do writeln(A);

    for (i,j) in {1..10,1..10} do
      writeln(i);

    // Shouldn't warn: j is underscore, aka "I don't care"
    for (i,_) in {1..10,1..10} do
      writeln(i);

    for i in 1..10 {
      for j in 1..10 {
        writeln(i);
      }
    }

    // test with unrelated attribute; previously this (wrongly) silenced warnigns
    @chplcheck.ignore("UnusedFormal")
    for i in 1..10 {}

    // silencing UnusedLoopIndex should definitely work, though.
    @chplcheck.ignore("UnusedLoopIndex")
    for i in 1..10 {}

    @chplcheck.ignore("UnusedLoopIndex")
    for (i,j) in {1..10,1..10} do
      writeln(i);
  }

  @chplcheck.ignore("UnusedFormal")
  proc myProcIgnored(A, B) {}

  myProc(1,2);
  myProcIgnored(1,2);
}
