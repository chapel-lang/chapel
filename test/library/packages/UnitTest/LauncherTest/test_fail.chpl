use UnitTest;

proc test1(test: borrowed Test) throws {
  var xND : [1..5,1..7] real;
  var yND : [{1..5,1..7}] real;
  test.assertNotEqual(xND,yND);
}

proc test2(test: borrowed Test) throws {
  test.assertTrue(false);
}

proc test3(test: borrowed Test) throws {
  test.skipIf(2==2,"Skipping Test 3");
  test.assertEqual(2,3.0);
}

UnitTest.main();
