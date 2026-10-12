//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use CommDiagnostics;

/* START_EXAMPLE_0 */
startVerboseComm();
// between start/stop calls, report comm ops initiated on any locale
stopVerboseComm();
/* STOP_EXAMPLE_0 */

resetCommDiagnostics();

/* START_EXAMPLE_1 */
startVerboseCommHere();
// between start/stop calls, report comm ops initiated on this locale
stopVerboseCommHere();
/* STOP_EXAMPLE_1 */

resetCommDiagnostics();

/* START_EXAMPLE_2 */
// (optional) if we counted previously, reset the counters to zero
resetCommDiagnostics();
startCommDiagnostics();
// between start/stop calls, count comm ops initiated on any locale
stopCommDiagnostics();
// retrieve the counts and report the results
writeln(getCommDiagnostics());
/* STOP_EXAMPLE_2 */

resetCommDiagnostics();

/* START_EXAMPLE_3 */
// (optional) if we counted previously, reset the counters to zero
resetCommDiagnosticsHere();
startCommDiagnosticsHere();
// between start/stop calls, count comm ops initiated on this locale
stopCommDiagnosticsHere();
// retrieve the counts and report the results
writeln(getCommDiagnosticsHere());
/* STOP_EXAMPLE_3 */

resetCommDiagnostics();
