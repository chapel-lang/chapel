module ArrowCpp {
  require 'ArrowInclude.chpl';
  use SysCTypes, CPtr, Time;
  use ArrowInclude;
  
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

    // why did this break?
    var g = c_getType(asd.c_str(), colName.c_str()): string;
    writeln(g);
      
    var C: [0..#15] int;
    readFilesByNameNew(C, [asd, asd], [10,5], colName);
    writeln(C);

    writeln(getArrSizeAndType([asd, asd]));
    
    // TODO: Get size and type
    // TODO: research that chunked array whatever
  }
}