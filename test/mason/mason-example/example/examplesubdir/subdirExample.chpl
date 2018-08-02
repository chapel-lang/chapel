
module subdirExample {

  use sampleModule;

  proc testSampleModule() {

    // Call function sampleModule that depends
    // on _MasonTest1
    testProc("Example within subdirectory Passed!");
  }

  proc main() {
    testSampleModule();
  }

}
 