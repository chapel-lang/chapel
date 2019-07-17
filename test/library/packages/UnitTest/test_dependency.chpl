use UnitTest;
proc A(test: Test) throws {
  test.assertTrue(true);
}

proc B(test: Test) throws {
  test.dependsOn(A);
}

proc C(test: Test) throws {
  test.dependsOn(A,B);
}

proc D(test: Test) throws {
  test.dependsOn(B,A);
}

UnitTest.runTest(D,C,B,A);