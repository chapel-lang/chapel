module ArrowCpp {
  use SysCTypes, CPtr, Time;
  require "cpp-arrow.h";
  require "cpp-arrow.o";

  config const NUMELEMS = 100_000;
  
  extern proc writeParquet(a);
  extern proc readParquet(a, b);
  
  proc main() {
    var t: Timer;
    t.start();
    writeParquet(NUMELEMS);
    var doneWrite = t.elapsed();
    
    var A: [0..#NUMELEMS] int;
    readParquet(c_ptrTo(A), NUMELEMS);
    var doneRead = t.elapsed();

    writeln("write took: ", doneWrite);

    writeln("read took: ", doneRead-doneWrite);
  }
}