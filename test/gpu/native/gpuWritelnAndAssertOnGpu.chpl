use GPU;

proc subFunc() {
  gpuWriteln(c_ptrToConst_helper("hello from function called by GPU!"):c_string);
  gpuWriteln(c_ptrToConst_helper("hello again from "):c_string, c_ptrToConst_helper(" function called by GPU!"):c_string);
}

writeln("Writeln from CPU");
on here.gpus[0] {
  var A : [0..10] int;
  foreach i in 0..10 {
    A[i] = i;
    assertOnGpu();
    if(i == 0) {
      gpuWriteln(c_ptrToConst_helper("hello from the GPU!"):c_string);
      gpuWriteln(c_ptrToConst_helper("hello from the GPU "):c_string, c_ptrToConst_helper("again!"):c_string);
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
