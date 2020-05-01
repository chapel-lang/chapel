/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
/*
  Holder for test result information.
  Test results are automatically managed by the TestLauncher, and do not 
  need to be explicitly manipulated by writers of tests.  
  Each instance holds the total number of tests run, and collections of 
  failures and errors that occurred among those test runs. The collections
  contain tuples of (testcase, exceptioninfo), where exceptioninfo is the
  formatted traceback of the error that occurred.
*/
module TestResult {
  private use List;
  class TestResult {
    type tupType = 2*string;
    var failures: list(tupType),
        errors: list(tupType),
        skipped: list(tupType);
    var testsRun = 0;
    var testsPassed = 0;
    var shouldStop = false;
    var separator1 = "="* 70,
        separator2 = "-"* 70;
    // called when a test ran
    proc testRan() {
      this.testsRun += 1;
    }
    
    /*Called when an error has occurred.*/
    proc addError(testName: string, fileName: string, errMsg: string) {
      this.testRan();
      var fileAdd = fileName + ": " + testName;
      this.errors.append((fileAdd, errMsg));
    }

    /*called when error occured */
    proc addFailure(testName: string, fileName: string, errMsg: string) {
      this.testRan();
      var fileAdd = fileName + ": " + testName;
      this.failures.append((fileAdd, errMsg));
    }

    /*Called when a test has completed successfully*/
    proc addSuccess(testName: string, fileName: string) {
      this.testRan();
      this.testsPassed += 1;
    }

    /*Called when a test is skipped.*/
    proc addSkip(testName: string, fileName: string, errMsg: string) {
      this.testRan();
      var fileAdd = fileName + ": " + testName;
      this.skipped.append((fileAdd, errMsg));
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
        writeln(flavour, " ", test);
        writeln(this.separator2);
        writeln(err);
      }
    }

    /* Function to print the result*/
    proc printResult(timeTaken: real) {
      var skipped = this.numSkippedTests();
      var run = this.testsRun - skipped;
      if this.testsRun != 0 {
        writeln("Ran ", run, " ", printTest(run)," in ",timeTaken," seconds");
        writeln();
        var infos: list((string));
        if testsPassed != 0 then 
          infos.append("passed = " + testsPassed: string);
        if !this.wasSuccessful() {
          write("FAILED");
          var failed = this.numFailedTests(),
            errored = this.numErroredTests();
          if failed then
            infos.append("failures = " + failed: string);
          if errored then
            infos.append("errors = " + errored: string);
        }
        else
          write("OK");
        if skipped then
          infos.append("skipped = " + skipped: string);
        if infos.size {
          write(" (");
          for info in infos do write(info, " ");
          writeln(")");
        }
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
}
