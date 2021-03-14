/** Doppelganger: See what happens when we pass an array into a constructor.
 **   Is the array copied into the constructed class or is it passed in as a
 **   reference?  At present, it is copied.
 **/
 
use Memory.Diagnostics;

param M = 2048;

class Doppelganger {
  var A: [1..M] int;
}

var prepre = memoryUsed();

// Create an array to pass to the constructor
var x: [1..M] int;

for xx in x do xx = 1;

var pre  = memoryUsed() - prepre;
var y    = new unmanaged Doppelganger(x);
var post = memoryUsed() - prepre;

if post >= 2*pre then
  writeln("Array was copied");
else
  writeln("Array was not copied");

delete y;
