use UnitTest;

// This test requires 8 locales
proc s1(test: Test) throws {
  test.addNumLocales(16);
}

// This test can run with 2-4 locales
proc s2(test: Test) throws {
  test.maxLocales(4);
  test.minLocales(2);
}

// This test can run with 8 or 16 locales
proc s3(test: Test) throws {
  test.addNumLocales(8,16);
}

// This test has min locales >  max locales
proc s4(test: Test) throws {
  test.maxLocales(5);
  test.minLocales(10);
}

UnitTest.runTest(s1,s2,s3,s4);
