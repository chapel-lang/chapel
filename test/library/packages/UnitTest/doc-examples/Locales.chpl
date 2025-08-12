//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use UnitTest;

/* START_EXAMPLE_1 */
proc test3(test: borrowed Test) throws {
  test.addNumLocales(5, 3);
}
/* STOP_EXAMPLE_1 */

/* START_EXAMPLE_2 */
proc test4(test: borrowed Test) throws {
  test.maxLocales(4);
  test.minLocales(2);
}
/* STOP_EXAMPLE_2 */

UnitTest.main();
