

// import module code from src
use sampleModule;

proc testSampleModule() {

  // Call function sampleModule that depends
  // on _MasonTest1
  testProc("Example with dependency outside module Passed!");
}

// call test function
proc main() {
  testSampleModule();
}

