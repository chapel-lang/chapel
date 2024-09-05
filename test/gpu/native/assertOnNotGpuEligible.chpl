// This test and assertOnFailToGpuize are siblings; this one tests
// @gpu.assertEligible, which does not have a runtime component.

use GPU;

config param failureMode = 8;
var globalVar = 0;

proc directlyRecursiveFunc(i:int) { if i > 0 then directlyRecursiveFunc(i-1); }
proc indirectlyRecursiveFunc(i:int) { if i > 0 then indirectlyRecursiveFunc2(i-1); }
proc indirectlyRecursiveFunc2(i:int) { if i > 0 then indirectlyRecursiveFunc(i-1); }
proc usesOutsideVar() { return globalVar; }

pragma "no gpu codegen"
proc funcMarkedNotGpuizableThatTriesToGpuize() {
  @gpu.assertEligible
  foreach i in 0..10 {
   
  }
}

pragma "no gpu codegen"
proc funcMarkedNotGpuizable() { }

on here.gpus[0] {
  if failureMode == 1 {
    funcMarkedNotGpuizableThatTriesToGpuize();
  }

  if failureMode == 2 {
    @gpu.assertEligible
    foreach i in 0..10 {
      funcMarkedNotGpuizable();
    }
  }

  if failureMode == 3 {
    @gpu.assertEligible
    foreach i in 0..10 {
      usesOutsideVar();
    }
  }

  // Also ensure that gpu.assertEligible does not fail
  // for the following (use failureMode >= 4
  // to run these tests):

  // calling a recursive function is allowed now
  @gpu.assertEligible
  foreach i in 0..10 {
    directlyRecursiveFunc(i);
  }

  @gpu.assertEligible
  foreach i in 0..10 {
    indirectlyRecursiveFunc(i);
  }

  // I want to ensure this works
  // with forall loops as well:
  @gpu.assertEligible
  forall i in 0..10 {
    directlyRecursiveFunc(i);
  }

  // And loops of a multidimensional array:
  {
    var A: [1..10, 1..10] int;
    @gpu.assertEligible
    foreach a in A {
      directlyRecursiveFunc(5);
    }
  }

  @gpu.assertEligible foreach i in 0..10 { }
  @gpu.assertEligible forall i in 0..10 { }
  var A: [1..10, 1..10] int;
  @gpu.assertEligible foreach a in A { }
  @gpu.assertEligible foreach idx in {0..10, 0..10} { }
  @gpu.assertEligible forall a in A { }
  @gpu.assertEligible forall idx in {0..10, 0..10} { }
}
