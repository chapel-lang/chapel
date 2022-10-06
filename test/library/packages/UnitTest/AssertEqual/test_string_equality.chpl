use UnitTest;

proc first_longer(test: borrowed Test) throws {
  test.assertEqual("Goodbye, Mars!", "Hello, World!");
}

proc equality_diff_lengths(test: borrowed Test) throws {
  test.assertEqual("aaaaa", "aa");
}

UnitTest.main();
