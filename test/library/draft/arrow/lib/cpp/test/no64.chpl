module ArrowCpp {
  require '../ArrowInclude.chpl';
  use SysCTypes, CPtr, Time, BlockDist;
  use ArrowInclude;

  extern proc writeParquet(a, b);
  extern proc readParquet(a, b);
  
  proc main() {
    writeParquet(100, 100);
    var a: [0..#100] int;
    readParquet(c_ptrTo(a), 100);
    writeln(a);
  }
}