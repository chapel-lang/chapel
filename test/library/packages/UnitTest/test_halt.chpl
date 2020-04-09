use UnitTest;

proc test1(test: borrowed Test) throws {
  test.assertTrue(true);
}

proc test2(test: borrowed Test) throws {
  var x: [1..0] int;
  writeln(x[0]);
  test.assertFalse(false);
}

proc test3(test: borrowed Test) throws {
  test.skipIf(3==2,"Skipping Test 3");
  test.assertEqual(2,3.0);
}

UnitTest.main();
