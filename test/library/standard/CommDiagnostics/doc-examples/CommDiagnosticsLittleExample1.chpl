//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
use CommDiagnostics;

proc main() {
  startVerboseComm();
  var x: int = 1;
  on Locales(1) {     // should execute_on a blocking task onto locale 1
    x = x + 1;        // should invoke a remote put and a remote get
  }
  stopVerboseComm();
}
/* STOP_EXAMPLE_0 */
