/* Documentation for CompflagsBug */
module CompflagsBug {

  config param testFlag = false;
  config param testFlagCmdLine = 0;
  config const testExecFlagCmdLine = "foo";

  proc main() {
    assert(testFlag); // set in Mason.toml
    assert(testFlagCmdLine == 1); // set via command line
    assert(testExecFlagCmdLine == "bar"); // set via command line
  }
}
