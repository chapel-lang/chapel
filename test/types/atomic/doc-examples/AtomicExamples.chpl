//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
var x: atomic int;
/* STOP_EXAMPLE_0 */

/* START_EXAMPLE_1 */
var y: atomic int = 1;
/* STOP_EXAMPLE_1 */

/* START_EXAMPLE_2 */
var one: int = 1;
var z = one : atomic int; // creates an `atomic int` initialized with 1
/* STOP_EXAMPLE_2 */

assert(z.read() == 1);

/* START_EXAMPLE_3 */
var a: atomic int = 1;
var b: atomic int = 2;

a = b; // equivalent to a.write(b.read())
/* STOP_EXAMPLE_3 */

// Verify that the comment is indeed true...
var a1: atomic int = 1;
var b1: atomic int = 2;
a1.write(b1.read());

assert(a.read() == 2 && a.read() == b.read());
assert(a1.read() == a.read());
assert(b1.read() == b.read());
