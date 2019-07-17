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

UnitTest.runTest(C,B,A);