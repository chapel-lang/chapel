//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use IO.FormattedIO;

/* START_EXAMPLE_1 */
writef("|%{#####}|\n", 2.0i);
     // outputs:
     //   |   2i|

writef("|%{#####.#}|\n", 2.0i);
     // outputs:
     //   |   2.0i|
/* STOP_EXAMPLE_1 */

/* START_EXAMPLE_2 */
writef("|%{#########}|\n", 1.0+2.0i);
     // outputs:
     //   |   1 + 2i|

writef("|%{#########.#}|\n", 1.0+2.0i);
     // outputs:
     //   | 1.0 + 2.0i|
/* STOP_EXAMPLE_2 */
