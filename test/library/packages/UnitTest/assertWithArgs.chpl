// assertWithArgs.chpl
use UnitTest;

// Test assertTrue with extra args
proc testTrueWithArgs(test: borrowed Test) throws {
  var x = 5;
  test.assertTrue(true, "This should pass with extra args:", x);
  
  // This should fail and show the message
  try {
    test.assertTrue(false, "Expected value:", x, "to be true");
    test.assertTrue(false, "Should not reach here");
  } catch e: AssertionError {
    // Expected to fail - check message contains our args
    test.assertTrue(e.message().find("Expected value:") >= 0);
  }
}

// Test assertFalse with extra args
proc testFalseWithArgs(test: borrowed Test) throws {
  var y = 10;
  test.assertFalse(false, "This should pass with extra args:", y);
  
  try {
    test.assertFalse(true, "Expected false but got true, value:", y);
    test.assertTrue(false, "Should not reach here");
  } catch e: AssertionError {
    test.assertTrue(e.message().find("value:") >= 0);
  }
}

// Test assertEqual with extra args
proc testEqualWithArgs(test: borrowed Test) throws {
  var a = 5, b = 5, c = 10;
  test.assertEqual(a, b, "Values match:", a, "and", b);
  
  try {
    test.assertEqual(a, c, "Expected a=", a, "to equal c=", c);
    test.assertTrue(false, "Should not reach here");
  } catch e: AssertionError {
    test.assertTrue(e.message().find("Expected a=") >= 0);
  }
}

// Test assertNotEqual with extra args
proc testNotEqualWithArgs(test: borrowed Test) throws {
  var x = 5, y = 10;
  test.assertNotEqual(x, y, "Values differ:", x, "and", y);
  
  try {
    test.assertNotEqual(x, x, "Expected different values, got:", x);
    test.assertTrue(false, "Should not reach here");
  } catch e: AssertionError {
    test.assertTrue(e.message().find("Expected different") >= 0);
  }
}

// Test assertGreaterThan with extra args
proc testGreaterThanWithArgs(test: borrowed Test) throws {
  var x = 10, y = 5;
  test.assertGreaterThan(x, y, "x=", x, "is greater than y=", y);
  
  try {
    test.assertGreaterThan(y, x, "Expected y=", y, "> x=", x);
    test.assertTrue(false, "Should not reach here");
  } catch e: AssertionError {
    test.assertTrue(e.message().find("Expected y=") >= 0);
  }
}

// Test assertLessThan with extra args
proc testLessThanWithArgs(test: borrowed Test) throws {
  var x = 5, y = 10;
  test.assertLessThan(x, y, "x=", x, "is less than y=", y);
  
  try {
    test.assertLessThan(y, x, "Expected y=", y, "< x=", x);
    test.assertTrue(false, "Should not reach here");
  } catch e: AssertionError {
    test.assertTrue(e.message().find("Expected y=") >= 0);
  }
}

// Test with multiple types
proc testMixedTypes(test: borrowed Test) throws {
  var i = 42;
  var s = "hello";
  var r = 3.14;
  
  test.assertTrue(true, "Mixed types:", i, s, r);
  
  try {
    test.assertEqual(i, 100, "int:", i, "string:", s, "real:", r);
  } catch e: AssertionError {
    test.assertTrue(e.message().find("string:") >= 0);
  }
}

UnitTest.main();
