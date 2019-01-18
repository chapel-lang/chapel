
module subdirectorytest {

  // import module code from src
  use sampleModule;

  proc testSampleModule() {

    // Call function sampleModule that depends
    // on _MasonTest1
    testProc("Test with dependency within subdirectory Passed!");
  }

  // call test function
  proc main() {
    testSampleModule();
  }

}
 