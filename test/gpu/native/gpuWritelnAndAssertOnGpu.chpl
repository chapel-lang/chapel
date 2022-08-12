use GPU;

proc subFunc() {
  gpuWriteln("hello from function called by GPU!".c_str());
  gpuWriteln("hello again from ".c_str(), " function called by GPU!".c_str());
}

writeln("Writeln from CPU");
on here.gpus[0] {
  var A : [0..10] int;
  foreach i in 0..10 {
    A[i] = i;
    assertOnGpu();
    if(i == 0) {
      gpuWriteln("hello from the GPU!".c_str());
      gpuWriteln("hello from the GPU ".c_str(), "again!".c_str());
      subFunc();
    }
  }
  // At this point the kernel should end and everything coming from gpuWriteln
  // should have been flushed to the terminal..

  writeln("Before next loop");

  foreach i in 0..10 {
    assertOnGpu();
    writeln("As of today this will throw us off the GPU");
  }

  writeln("!!! This message should not be displayed unless there's a bug in");
  writeln("    assertOnGpu or we fixed things so that writeln can be Gpuized. If that's");
  writeln("    fixed then cool we can get rid of gpuWriteln and this test!");
}
