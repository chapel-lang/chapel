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


UnitTest.main();
