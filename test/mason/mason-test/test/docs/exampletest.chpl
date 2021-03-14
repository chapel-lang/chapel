// This test is intent for identify if the example from the docs breaks.
// https://chapel-lang.org/docs/tools/mason/mason.html#testing-your-package

use UnitTest;

config const testParam: bool = true;

proc myTest(test: borrowed Test) throws{
  test.assertTrue(testParam);
}

UnitTest.main();
