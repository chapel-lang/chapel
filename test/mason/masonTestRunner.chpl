// test the usage of 'mason test' as a standalone test runner

use MasonTest;

enum TestCase {
  Basic,
  Fail,
  Filter1,
  Filter2,
  FilterDepends1,
  FilterDepends2
}
use TestCase;
config const test = Basic;

proc main() {
  // run multiple times since `masonTest` calls exit()
  if test == Basic {
    masonTest(["test", "masonTestRunner-MyTest.chpl"]);
  } else if test == Fail {
    masonTest(["test", "--", "masonTestRunner-MyTest.chpl", "--set", "fail=true"]);
  } else if test == Filter1 {
    masonTest(["test", "masonTestRunner-MyTest.chpl", "--filter", "test1"]);
  } else if test == Filter2 {
    masonTest(["test", "masonTestRunner-MyTest.chpl", "--filter", "test1|test2"]);
  } else if test == FilterDepends1 {
    masonTest(["test", "masonTestRunner-MyTest.chpl", "--filter", "iHaveDependents"]);
  } else if test == FilterDepends2 {
    masonTest(["test", "masonTestRunner-MyTest.chpl", "--filter", "iHaveDependents|iAmADependency"]);
  }
}

