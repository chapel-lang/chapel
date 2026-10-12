use UnitTest;

proc test1(test: borrowed Test) throws {
  test.assertTrue(true);
}

proc test2(test: borrowed Test) throws {
  test.assertFalse(false);
}

UnitTest.main();
