module ArrowCpp {
  use SysCTypes, CPtr;
  require "cpp-arrow.h";
  require "cpp-arrow.o";

  extern proc writeParquet();
  extern proc readParquet(a);
  
  proc main() {
    writeParquet();
    var A: [0..#100000] int;
    readParquet(c_ptrTo(A));
  }
}