module ArrowCpp {
  require '../ArrowInclude.chpl';
  use SysCTypes, CPtr, Time, BlockDist;
  use ArrowInclude;
  use Time;

  config const fileSize = 100_000;
  
  proc main() {
    writeln(getVersionInfo());
    
    var A = newBlockArr(0..#fileSize, int);
    coforall loc in A.targetLocales() do on loc {
      for dom in A.localSubdomains() do
        for i in dom do
          A[i] = i;
    }

    writeDistArrayToParquet(A, "test-file", "test-row", (1024*128*512));
    writeDistArrayToParquet(A, "secondtest-file", "test-row", (1024*128*512));
    
    var filenames = ["test-file_LOCALE0.parquet", "secondtest-file_LOCALE0.parquet"];
    var fromC: [0..#fileSize] int;
    var fromC2: [0..#fileSize] int;

    var t: Timer;
    t.start();
    c_lowLevelRead(filenames[0].c_str(), c_ptrTo(fromC),"test-row".c_str(), fileSize);
    var doneLow = t.elapsed();

    c_readColumnByName(filenames[1].c_str(), c_ptrTo(fromC2),"test-row".c_str(), fileSize);
    var doneReg = t.elapsed() - doneLow;

    /*for i in 0..#fileSize do
      if fromC[i] != fromC2[i] then
      writeln(fromC[i], " != ", fromC2[i]);*/
    writeln(fromC[fileSize-25]);
    writeln(fromC2[fileSize-25]);

    writeln("Low level read took: ", doneLow*1000);
    writeln("Regular read took: ", doneReg*1000);
  }
}
