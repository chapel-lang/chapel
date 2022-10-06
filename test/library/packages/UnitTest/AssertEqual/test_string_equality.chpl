use UnitTest;

proc first_longer(test: borrowed Test) throws {
  test.assertEqual("Goodbye, Mars!", "Hello, World!");
}

UnitTest.main();