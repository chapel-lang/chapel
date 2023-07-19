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
}

// Should produce runtime error
foreach i in 0..10 {
  assertOnGpu();
}

writeln("!!! This message should not be displayed unless there's a bug in");
writeln("    assertOnGpu.");
