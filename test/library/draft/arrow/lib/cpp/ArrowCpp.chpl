module ArrowCpp {
  use SysCTypes;
  require "cpp-arrow.h";
  require "cpp-arrow.o";

  extern proc callFoo(i): c_int;
  extern proc writeParquet();
  
  proc main() {
    writeParquet();
  }
}