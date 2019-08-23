use UnitTest;

// This test requires 16 locales
proc s1(test: borrowed Test) throws {
  test.addNumLocales(16);
}

// This test can run with 2-4 locales
proc s2(test: borrowed Test) throws {
  test.maxLocales(4);
  test.minLocales(2);
}

// This test can run with 8 or 16 locales
proc s3(test: borrowed Test) throws {
  test.addNumLocales(16,8);
}

// This test has min locales >  max locales
proc s4(test: borrowed Test) throws {
  test.maxLocales(5);
  test.minLocales(10);
}

// This test has only max locales
proc s5(test: borrowed Test) throws {
  test.maxLocales(5);
}

//This test has only min locales
proc s6(test: borrowed Test) throws {
  test.minLocales(6);
}


//This test has addNumLocales Multiple Times
proc s7(test: borrowed Test) throws {
  test.addNumLocales(8,16);
  test.addNumLocales(16,7);
}

//This test has maxLocales < 1
proc s8(test: borrowed Test) throws {
  test.maxLocales(0);
}

//in-order tests
proc s9(test: borrowed Test) throws {
  test.addNumLocales(8,16);
}

// this test halts
proc s10(test: borrowed Test) throws {
  var a: [1..0] int;
  writeln(a[0]);
}

// this depend on test s9 which depends on s10 which halts
proc s11(test: borrowed Test) throws {
  test.dependsOn(s14,s1);
}

// this depends on a test that pass
proc s12(test: borrowed Test) throws {
  test.dependsOn(s2);
}

// this depends on a test which fails
proc s13(test: borrowed Test) throws {
  test.dependsOn(s8);
}

// depends on a test that halts
proc s14(test: borrowed Test) throws {
  test.dependsOn(s10);
}
UnitTest.main();
