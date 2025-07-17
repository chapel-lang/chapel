//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
// The second formal of 'baz' is anonymous and cannot be used.
proc baz(x: int, _: int): int { return x + 1; }
/* STOP_EXAMPLE_0 */

var x = baz(2, 2);
assert(x == 3);
