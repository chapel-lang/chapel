use UnitTest;

proc test1(test: borrowed Test) throws {
  test.assertTrue(true);
}

proc test2(test: borrowed Test) throws {
  test.assertFalse(false);
}

proc test3(test: borrowed Test) throws {
  test.skip("Skipping Test 3");
  test.assertEqual(2,3.0);
}

UnitTest.runTest(test1,test2,test3);
