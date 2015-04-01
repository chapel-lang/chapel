use myrecord;

// Declare some global variables.
var global1: R; // default initialized

var global2: R = new R(x=20); // constructor initialized


global1.verify();

global2.verify();
assert(global2.x == 20);


