/** Doppelganger: See what happens when we pass an array into a constructor.
 **   Is the array copied into the constructed class or is it passed in as a
 **   reference?  At present, it is copied.
 **/
 
use Memory;

param M = 2048;

class Doppelganger {
  var A: [1..M] int;
}

// Create an array to pass to the constructor
var x: [1..M] int = 1;

var pre  = memoryUsed();
var y    = Doppelganger(x);
var post = memoryUsed();

if post >= 2*pre then
  writeln("Array was copied");
else
  writeln("Array was not copied");
