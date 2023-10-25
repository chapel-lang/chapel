use GpuDiagnostics;

proc check(ref Arr) {
  var A = [2, 3, 4, 5, 6];

  for (expected, actual) in zip(A, Arr) {
    if expected != actual then return false;
  }
  return true;
}

startGpuDiagnostics();

{
  var x = 10;
  coforall l in Locales do on l {
    cobegin {
      coforall g in here.gpus do on g {
        var A = [1, 2, 3, 4, 5];
        foreach a in A do a += 1;

        assert(check(A)); // not in slide
      }
      {
        var A = [1, 2, 3, 4, 5];
        foreach a in A do a += 1;

        assert(check(A)); // not in slide
      }
    }
  }
  writeln(x);

}

stopGpuDiagnostics();
assertGpuDiags(kernel_launch=numLocales*here.gpus.size);
