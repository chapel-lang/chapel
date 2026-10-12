// test the usage of 'mason test' as a standalone test runner

use MasonTest;

enum TestCase {
  Basic,
  Filter1,
  Filter3,
  Filter1OnlyA,
}
use TestCase;
config const test = Basic;

proc main() {
  // run multiple times since `masonTest` calls exit()
  if test == Basic {
    masonTest(["test"]);
  } else if test == Filter1 {
    masonTest(["test", "--filter", "test1"]);
  } else if test == Filter3 {
    masonTest(["test", "--filter", "test3"]);
  } else if test == Filter1OnlyA {
    masonTest(["test", "--filter", "test1", "test/testA.chpl"]);
  }
}

