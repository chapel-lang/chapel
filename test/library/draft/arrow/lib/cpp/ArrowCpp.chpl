module ArrowCpp {
  use SysCTypes;
  require "cpp-arrow.h";
  require "cpp-arrow.o";

  extern proc writeParquet();
  
  proc main() {
    writeParquet();
  }
}