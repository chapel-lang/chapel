//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use IO.FormattedIO;

/* START_EXAMPLE_1 */
writef("n:%{###.###}\n", 1.2349);
     // outputs:
     // n:  1.235
/* STOP_EXAMPLE_1 */

/* START_EXAMPLE_2 */
writef("%{.##}\n", 0.777);
     // outputs:
     // 0.78
/* STOP_EXAMPLE_2 */
