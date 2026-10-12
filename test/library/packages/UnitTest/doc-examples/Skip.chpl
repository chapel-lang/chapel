//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//


/* START_EXAMPLE */
use UnitTest;

/* calculates factorial */
proc factorial(x: int): int {
  return if x == 0 then 1 else x * factorial(x - 1);
}

/*Conditional skip*/
proc test1(test: borrowed Test) throws {
  test.skipIf(factorial(0) != 1, "Base condition is wrong in factorial");
  test.assertTrue(factorial(5) == 120);
}

/*Unconditional skip*/
proc test2(test: borrowed Test) throws {
  test.skip("Skipping the test directly");
}

UnitTest.main();
/* STOP_EXAMPLE */
