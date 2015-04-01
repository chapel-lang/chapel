use myrecord;

// Declare some global variables.
var global1: R; // default initialized

// This version does not work because the constructor
// is returning by value (rather than being a method).
//var global2: R = new R(x=20); // constructor initialized

var global2: R;
global2.init(x = 20);

global1.verify();

global2.verify();
assert(global2.x == 20);


