// test the usage of 'mason test' as a standalone test runner

use MasonTest;

enum TestCase {
  Basic,
  Fail,
}
use TestCase;
config param test = Basic;

proc main() {
  // run multiple times since `masonTest` calls exit()
  if test == Basic {
    masonTest(["test", "masonTestRunner-MyTest.chpl"]);
  } else if test == Fail {
    masonTest(["test", "--", "masonTestRunner-MyTest.chpl", "--set", "fail=true"]);
  }
}

