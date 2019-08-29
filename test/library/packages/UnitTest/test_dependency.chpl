use UnitTest;
proc A(test: borrowed Test) throws {
  test.assertTrue(true);
}

proc B(test: borrowed Test) throws {
  test.dependsOn(A);
}

proc C(test: borrowed Test) throws {
  test.dependsOn(A,B);
}

proc D(test: borrowed Test) throws {
  test.dependsOn(B,A);
}

UnitTest.main();
