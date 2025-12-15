/* Documentation for CompflagsBug */
module CompflagsBug {

  config param testFlag = false;

  proc main() {
    assert(testFlag); // we expect Mason to set this to true
  }
}
