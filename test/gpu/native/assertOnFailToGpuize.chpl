use GPU;

config param failureMode = 8;
var globalVar = 0;

proc directlyRecursiveFunc(i:int) { if i > 0 then directlyRecursiveFunc(i-1); }
proc indirectlyRecursiveFunc(i:int) { if i > 0 then indirectlyRecursiveFunc2(i-1); }
proc indirectlyRecursiveFunc2(i:int) { if i > 0 then indirectlyRecursiveFunc(i-1); }
proc usesOutsideVar() { return globalVar; }

pragma "no gpu codegen"
proc funcMarkedNotGpuizableThatTriesToGpuize() {
  foreach i in 0..10 {
    assertOnGpu();
  }
}

pragma "no gpu codegen"
proc funcMarkedNotGpuizable() { }

on here.gpus[0] {
  if failureMode == 1 {
    funcMarkedNotGpuizableThatTriesToGpuize();
  }

  if failureMode == 2 {
    foreach i in 0..10 {
      assertOnGpu();
      funcMarkedNotGpuizable();
    }
  }

  if failureMode == 3 {
    foreach i in 0..10 {
      assertOnGpu();
      usesOutsideVar();
    }
  }

  // Also ensure that assertOnGpu does not fail
  // for the following (use failureMode >= 4
  // to run these tests):

  // calling a recursive function is allowed now
  foreach i in 0..10 {
    assertOnGpu();
    directlyRecursiveFunc(i);
  }

  foreach i in 0..10 {
    assertOnGpu();
    indirectlyRecursiveFunc(i);
  }

  // I want to ensure this works
  // with forall loops as well:
  forall i in 0..10 {
    assertOnGpu();
    directlyRecursiveFunc(i);
  }

  // And loops of a multidimensional array:
  {
    var A: [1..10, 1..10] int;
    foreach a in A {
      assertOnGpu();
      directlyRecursiveFunc(5);
    }
  }

  foreach i in 0..10 { assertOnGpu(); }
  forall i in 0..10 { assertOnGpu(); }
  var A: [1..10, 1..10] int;
  foreach a in A { assertOnGpu(); }
  foreach idx in {0..10, 0..10} { assertOnGpu(); }
  forall a in A { assertOnGpu(); }
  forall idx in {0..10, 0..10} { assertOnGpu(); }
}
