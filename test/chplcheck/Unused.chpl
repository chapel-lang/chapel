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
  }
  myProc(1,2);
}
