use UnitTest;

proc test(test: borrowed Test) throws {
  test.assertTrue(true);
   test.assertFalse(false);
}

UnitTest.main();
