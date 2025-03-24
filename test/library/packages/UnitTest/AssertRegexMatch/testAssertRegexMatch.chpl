use UnitTest;
use Regex;

proc assertRegexMatchTestStringPass(test: borrowed Test) throws {
  var str = "test";
  test.assertRegexMatch(str, ".*es.*");
}

proc assertRegexMatchTestStringFail(test: borrowed Test) throws {
  var str = "test";
  test.assertRegexMatch(str, ".*ES.*");
}

proc assertRegexMatchTestObjectPass(test: borrowed Test) throws {
  var str = "test";
  var re = new regex(".*es.*");
  test.assertRegexMatch(str, re);
}

proc assertRegexMatchTestObjectFail(test: borrowed Test) throws {
  var str = "test";

  var re = new regex(".*ES.*");
  test.assertRegexMatch(str, re);
}

UnitTest.main();
