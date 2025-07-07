//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use CTypes;

{
/* START_EXAMPLE_0 */
var x = new c_array(c_int, 8);
if x.eltType == c_int then writeln("x is an array of c_int");
/* STOP_EXAMPLE_0 */

compilerAssert(x.eltType == c_int);
}

{
/* START_EXAMPLE_1 */
var x = new c_array(c_int, 8);
writeln(x.size); // will print '8'
/* STOP_EXAMPLE_1 */

assert(x.size == 8);
}
