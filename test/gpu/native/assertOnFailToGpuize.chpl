use GPU;

config param failureMode = 8;
var globalVar = 0;

proc directlyRecursiveFunc() { directlyRecursiveFunc(); }
proc indirectlyRecursiveFunc() { indirectlyRecursiveFunc2(); }
proc indirectlyRecursiveFunc2() { indirectlyRecursiveFunc(); }
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
    foreach i in 0..10 {
      assertOnGpu();
      directlyRecursiveFunc();
    }
  }

  if failureMode == 2 {
    foreach i in 0..10 {
      assertOnGpu();
      indirectlyRecursiveFunc();
    }
  }

  if failureMode == 3 {
    funcMarkedNotGpuizableThatTriesToGpuize();
  }

  if failureMode == 4 {
    foreach i in 0..10 {
      assertOnGpu();
      funcMarkedNotGpuizable();
    }
  }

  if failureMode == 5 {
    foreach i in 0..10 {
      assertOnGpu();
      usesOutsideVar();
    }
  }

  // I want to ensure this works
  // with forall loops as well:
  if failureMode == 6 {
    forall i in 0..10 {
      assertOnGpu();
      directlyRecursiveFunc();
    }
  }

  // And loops of a multidimensional array:
  if failureMode == 7 {
    var A: [1..10, 1..10] int;
    foreach a in A {
      assertOnGpu();
      directlyRecursiveFunc();
    }
  }

  // Also ensure that assertOnGpu does not fail
  // for the following (use failureMode >= 8
  // to run these tests):
  foreach i in 0..10 { assertOnGpu(); }
  forall i in 0..10 { assertOnGpu(); }
  var A: [1..10, 1..10] int;
  foreach a in A { assertOnGpu(); }
  foreach idx in {0..10, 0..10} { assertOnGpu(); }
  forall a in A { assertOnGpu(); }
  forall idx in {0..10, 0..10} { assertOnGpu(); }
}
