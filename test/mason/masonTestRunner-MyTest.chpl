use UnitTest;

config param fail = false;

proc test1(test: borrowed Test) throws {
  test.assertTrue(true);
}
proc test2(test: borrowed Test) throws {
  test.assertFalse(false);
  if fail {
    test.assertTrue(false);
  }
}

proc iAmADependency(test: borrowed Test) throws {
  test.assertTrue(true);
}
proc iHaveDependents(test: borrowed Test) throws {
  test.dependsOn(iAmADependency);
  test.assertTrue(true);
}

UnitTest.main();
