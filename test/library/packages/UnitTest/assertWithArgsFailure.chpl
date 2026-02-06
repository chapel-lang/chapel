use UnitTest;

proc testFailureMessage(test: borrowed Test) throws {
  var x = 5, y = 10;
  // This will fail and show our custom message
  test.assertEqual(x, y, "Expected x=", x, "to equal y=", y);
}

UnitTest.main();