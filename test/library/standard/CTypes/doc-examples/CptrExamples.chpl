//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use CTypes;

record someRecord { var x: int; }

{
/* START_EXAMPLE_0 */
var r = new someRecord();
var x = c_ptrTo(r);
if x then writeln("x is not nil");
if !x then writeln("x is nil");
/* STOP_EXAMPLE_0 */
}

class SomeClass { var x: int; }

{
/* START_EXAMPLE_1 */
var c: owned SomeClass? = nil;
var x = c_ptrTo(c);
writeln(x); // outputs nil
/* STOP_EXAMPLE_1 */
}

{
/* START_EXAMPLE_2 */
var foo: c_int = 0;
var x = c_ptrTo(foo);
if x.eltType == c_int then writeln("x is a c_int pointer");
/* STOP_EXAMPLE_2 */
}

{
/* START_EXAMPLE_3 */
var foo: c_int = 0;
var x = c_ptrToConst(foo);
if x.eltType == c_int then writeln("x is a const c_int pointer");
/* STOP_EXAMPLE_3 */
}
