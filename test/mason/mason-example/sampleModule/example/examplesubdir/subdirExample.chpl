
module subdirExample {

  use sampleModule;
  config const exConfig: string;

  proc testSampleModule() {

    // Call function sampleModule that depends
    // on _MasonTest1
    testProc("Example within subdirectory Passed!");
    writeln(exConfig);
  }

  proc main() {
    testSampleModule();
  }

}
 