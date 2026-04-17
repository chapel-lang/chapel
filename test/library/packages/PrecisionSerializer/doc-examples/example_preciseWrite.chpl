//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
/* START_EXAMPLE */
use IO, PrecisionSerializer;

var x = [i in 1..5] i + 0.123456789;
stdout.withSerializer(new precisionSerializer(precision=7, padding=12)).write(x);
/* STOP_EXAMPLE */

write("\n", x);
