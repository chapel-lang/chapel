
config const upper = true;
config const stride = 2;

proc main() {
  
  for (DG1, ) in MyBlock() {
    writeln(DG1);
  }
}


iter MyBlock() {
  var start   =  1;
  var stop    = 10;
  var blksize =  2;
  var hi: int;

  for i in 1..10 by stride {
    hi = min(i+blksize-1,stop);
    if upper {
      var D1 = {start..i-1,i..hi};
      yield (D1, );
    }
    else {
      var D1 = {i..hi,start..i-1};
      yield (D1, );
    }
  
  }
}

 
