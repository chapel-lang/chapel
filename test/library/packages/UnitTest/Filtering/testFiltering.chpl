use UnitTest;

proc testA(test: borrowed Test) throws {
  test.assertTrue(true);
}

proc testB(test: borrowed Test) throws {
  test.assertFalse(false);
}

proc failingTest(test: borrowed Test) throws {
  test.assertFalse(true);
}

proc iAmADependency(test: borrowed Test) throws {
  test.assertTrue(true);
}
proc iHaveDependents(test: borrowed Test) throws {
  test.dependsOn(iAmADependency);
  test.assertTrue(true);
}


UnitTest.main();
