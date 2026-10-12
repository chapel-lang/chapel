//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
/* START_EXAMPLE */
use IO;

var x: int;
var y: real;
/* reading into variable expressions, returning
   true if the values were read, false on EOF */
var ok:bool = read(x, y);

/* reading via a single type argument */
x = read(int);
y = read(real);

/* reading via multiple type arguments */
(x, y) = read(int, real);
/* STOP_EXAMPLE */

writeln("ok = ", ok);
writeln("x = ", x);
writeln("y = ", y);
