module ArrowCpp {
  require '../ArrowInclude.chpl';
  use SysCTypes, CPtr, Time, BlockDist;
  use ArrowInclude;

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
    
    var filenames = ["test-file_LOCALE0.parquet"];
    var fromC: [0..#fileSize] int;
    c_lowLevelRead(filenames[0].c_str(), c_ptrTo(fromC),"test-row".c_str(), fileSize);
    writeln(fromC);

    //var (sizes, ty) = getArrSizeAndType(filenames);
  
    // var B = newBlockArr(0..#(+ reduce sizes), int);
  }
}
