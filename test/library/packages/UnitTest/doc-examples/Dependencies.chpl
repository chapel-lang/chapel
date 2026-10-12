//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//


/* START_EXAMPLE */
use UnitTest;
use List;

var factorials: list(int);

// calculates factorial
proc factorial(x: int): int {
  return if x == 0 then 1 else x * factorial(x - 1);
}

proc testFillFact(test: borrowed Test) throws {
  test.skipIf(factorial(0) != 1, "Base condition is wrong in factorial");
  for i in 1..10 do
    factorials.pushBack(factorial(i));
}

proc testSumFact(test: borrowed Test) throws {
  test.dependsOn(testFillFact);
  var s = 0;
  for i in factorials.indices do
    s += factorials[i];
  test.assertGreaterThan(s, 0);
}

UnitTest.main();
/* STOP_EXAMPLE */
