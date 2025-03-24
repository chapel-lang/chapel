use UnitTest;

proc assertRegexMatchTestPass(test: borrowed Test) throws {

  var str = "test";
  test.assertRegexMatch(str, ".*es.*");
}

proc assertRegexMatchTestFail(test: borrowed Test) throws {
  var str = "test";
  test.assertRegexMatch(str, ".*ES.*");
}

UnitTest.main();
