//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use UnitTest;
/* START_EXAMPLE */
proc test_square(test: borrowed Test) throws {
  test.addNumLocales(5);
  var A: [Locales.domain] int;
  coforall i in 0..<numLocales with (ref A) {
    on Locales(i) {
      A[i] = (i+1) * (i+1);
    }
  }
  test.assertTrue(A[4] == 25);
}
/* STOP_EXAMPLE */
UnitTest.main();
