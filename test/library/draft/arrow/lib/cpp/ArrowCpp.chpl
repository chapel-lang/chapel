module ArrowCpp {
  use SysCTypes, CPtr, Time;
  require "cpp-arrow.h";
  require "cpp-arrow.o";

  config const NUMELEMS = 100_000;
  config const ROWGROUPS = 512*1024*128; // 512 mb
  
  extern proc writeParquet(a, b);
  extern proc readParquet(a, b);
  extern proc c_doSize(a): int;
  extern proc c_readColumnByIndex(a,b,c,d);
  extern proc c_readColumnByName(a,b,c,d);
  extern proc c_getType(a,b): c_string;
  
  proc main() {
    var t: Timer;
    t.start();
    writeParquet(NUMELEMS, ROWGROUPS);
    var doneWrite = t.elapsed();
    
    var A: [0..#NUMELEMS] int;
    readParquet(c_ptrTo(A), NUMELEMS);
    var doneRead = t.elapsed();

    writeln("write took: ", doneWrite);

    writeln("read took: ", doneRead-doneWrite);

    var asd = "test-cpp-file.parquet";
    var size = c_doSize(asd.c_str());
    writeln(size);
    var doneSize = t.elapsed();

    writeln("size took: ", doneSize-doneRead);

    var B: [0..#NUMELEMS] int;
    var colName = "int-col";
    c_readColumnByName(asd.c_str(), c_ptrTo(B), colName.c_str(), size);

    writeln(B[534..#12]);

    var g = c_getType(asd.c_str(), colName.c_str()):string;
    writeln(g);
    
    // TODO: Get size and type
    // TODO: research that chunked array whatever
  }
}