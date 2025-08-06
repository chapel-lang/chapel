foreach i in 0..10 { callSomeExtern(); }  // [*]
// [*] We want an ineligible loop on line 1, 5, 10, and 100 
//     In order to verify that we're sorting by line number correctly and not just by
//     using string comparison (e.g. foo.chpl:5 should be sorted before foo.chpl:10 )
foreach i in 0..10 { callSomeExtern(); } // [*]

extern { static void gpuIneligibleExtern(void) { }; }
proc callSomeExtern() { gpuIneligibleExtern(); }

foreach i in 0..10 { callSomeExtern(); }  // [*]
use GPU;

var globalVar = 0;

proc directlyRecursiveFunc(i:int) { if i > 0 then directlyRecursiveFunc(i-1); }
proc indirectlyRecursiveFunc(i:int) { if i > 0 then indirectlyRecursiveFunc2(i-1); }
proc indirectlyRecursiveFunc2(i:int) { if i > 0 then indirectlyRecursiveFunc(i-1); }
proc usesOutsideVar() { return globalVar; }

pragma "no gpu codegen"
proc funcMarkedNotGpuizableThatTriesToGpuize() {
  foreach i in 0..10 {
  }
}

pragma "no gpu codegen"
proc funcMarkedNotGpuizable() { return 1; }

on here.gpus[0] {
  funcMarkedNotGpuizableThatTriesToGpuize();

  foreach i in 0..10 {
    funcMarkedNotGpuizable();
  }

  foreach i in 0..10 {
    usesOutsideVar();
  }

  // calling a recursive function is allowed now
  foreach i in 0..10 {
    directlyRecursiveFunc(i);
  }

  foreach i in 0..10 {
    indirectlyRecursiveFunc(i);
  }

  // I want to ensure this works
  // with forall loops as well:
  forall i in 0..10 {
    directlyRecursiveFunc(i);
  }

  // And loops of a multidimensional array:
  {
    var A: [1..10, 1..10] int;
    foreach a in A {
      directlyRecursiveFunc(5);
    }
  }

  foreach i in 0..10 { }
  forall i in 0..10 { }
  var A: [1..10, 1..10] int;
  foreach a in A { }
  foreach idx in {0..10, 0..10} { }
  forall a in A { }
  forall idx in {0..10, 0..10} { }
}





























foreach i in 0..10 { callSomeExtern(); }  // [*]
