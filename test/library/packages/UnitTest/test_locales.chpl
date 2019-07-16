use UnitTest;

// This test requires 16 locales
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
  test.addNumLocales(16,8);
}

// This test has min locales >  max locales
proc s4(test: Test) throws {
  test.maxLocales(5);
  test.minLocales(10);
}

// This test has only max locales
proc s5(test: Test) throws {
  test.maxLocales(5);
}

//This test has only min locales
proc s6(test: Test) throws {
  test.minLocales(6);
}


//This test has addNumLocales Multiple Times
proc s7(test: Test) throws {
  test.addNumLocales(8,16);
  test.addNumLocales(16,7);
}

//This test has maxLocales < 1
proc s8(test: Test) throws {
  test.maxLocales(0);
}

//in-order tests
proc s9(test: Test) throws {
  test.addNumLocales(8,16);
}

UnitTest.runTest(s1,s2,s3,s4,s5,s6,s7,s8,s9);
