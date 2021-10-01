module ArrowCpp {
  require '../ArrowInclude.chpl';
  use SysCTypes, CPtr, Time, BlockDist;
  use ArrowInclude;
  
  proc main() {
    var A = newBlockArr(0..#100_000, int);
    coforall loc in A.targetLocales() do on loc {
      for dom in A.localSubdomains() do
        for i in dom do
          A[i] = i;
    }
    //writeDistArrayToParquet(A, "test-file", 0, (1024*128*512));

    var asd = [i in 0..#100] i;
    c_writeColumnToParquet("asd.parquet".c_str(), c_ptrTo(asd), 0, "asd".c_str(), 100, 100000);
    
    
    var filenames = ["asd.parquet"];
    var fromC: [0..#100] int;
    c_readColumnByName("asd.parquet", c_ptrTo(fromC), "asd", 100);
    writeln(fromC);

    //var (sizes, ty) = getArrSizeAndType(filenames);
  
    // var B = newBlockArr(0..#(+ reduce sizes), int);
  }
}
