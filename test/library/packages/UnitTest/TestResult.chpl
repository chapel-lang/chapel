/*
  Holder for test result information.
  Test results are automatically managed by the TestRunner, and do not 
  need to be explicitly manipulated by writers of tests.  
  Each instance holds the total number of tests run, and collections of 
  failures and errors that occurred among those test runs. The collections
  contain tuples of (testcase, exceptioninfo), where exceptioninfo is the
  formatted traceback of the error that occurred.
*/
class TestResult {
  type tupType = 2*string;
  var failures: [1..0] tupType,
      errors: [1..0] tupType,
      skipped: [1..0] tupType;
  var testsRun = 0;
  var shouldStop = false;
  var separator1 = "="* 70,
      separator2 = "-"* 70;
  // called when a test if about to run
  proc startTest() {
    this.testsRun += 1;
  }
  
  /*Called when an error has occurred.*/
  proc addError(test: string, errMsg: string) {
    this.errors.push_back((test, errMsg));
  }

  /*called when error occured */
  proc addFailure(test: string, errMsg: string) {
    this.failures.push_back((test, errMsg));
  }

  /*Called when a test has completed successfully*/
  proc addSuccess(test: string) { }

  /*Called when a test is skipped.*/
  proc addSkip(test: string, reason: string) {
    this.skipped.push_back((test, reason));
  }

  /*Tells whether or not this result was a success.*/
  proc wasSuccessful() {
    return this.failures.size == 0 && this.errors.size == 0;
  }
  
  /* Indicates that the tests should be aborted. */
  proc stop() {
    this.shouldStop = true;
  }

  /*Count of test skipped*/
  proc numSkippedTests() {
    return this.skipped.size;
  }

  /*Count of test failed*/
  proc numFailedTests() {
    return this.failures.size;
  }

  /*Count of tests giving error*/
  proc numErroredTests() {
    return this.errors.size;
  }

  proc printErrors() {
    writeln();
    this.printErrorList("ERROR", this.errors);
    this.printErrorList("FAIL", this.failures);
    this.printErrorList("SKIPPED", this.skipped);
  }

  proc printErrorList(flavour, errors) {
    for (test, err) in errors {
      writeln(this.separator1);
      writeln(flavour," ",test);
      writeln(this.separator2);
      writeln(err);
    }
  }

  /* Function to print the result*/
  proc printResult() {
    var skipped = this.numSkippedTests();
    var run = this.testsRun - skipped;
    if this.testsRun!=0 {
      writeln("Run "+ run +" "+printTest(run));
      writeln();
      var infos: [1..0](string);
      if !this.wasSuccessful() {
        write("FAILED");
        var failed = this.numFailedTests(),
          errored = this.numErroredTests();
        if failed then
          infos.push_back("failures = "+failed);
        if errored then
          infos.push_back("errors = "+errored);
      }
      else
        stdout.write("OK");
      if skipped then
        infos.push_back("skipped = "+skipped);
      if infos.size {
        stdout.write(" ");
        for info in infos do stdout.write(info," ");
      }
      stdout.write("\n");
      // for value in this.failures do stdout.writeln(value);
      // for value in this.skipped do stdout.writeln(value);
    }
    else {
      writeln("No Tests Found");
    }
  }

  proc printTest(count) {
    if count > 1 {
      return "tests";
    }
    return "test";
  }
}