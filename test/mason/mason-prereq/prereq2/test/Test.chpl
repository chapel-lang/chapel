use UnitTest;

use prereq2;

proc test1(test: borrowed Test) throws {
  hello();
  test.assertEqual(getMyParam(), 2);
}

UnitTest.main();
