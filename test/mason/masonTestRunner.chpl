// test the usage of 'mason test' as a standalone test runner

use MasonTest;

enum TestCase {
  Basic,
  Fail,
  Filter1,
  FilterAll,
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
  } else if test == FilterAll {
    masonTest(["test", "masonTestRunner-MyTest.chpl", "--filter", "test1|test2"]);
  }
}

