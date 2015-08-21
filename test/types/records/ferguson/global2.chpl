use myrecord;

// This version does not work because the constructor
// is returning by value (rather than being a method).
var global2: R = new R(x=20); // constructor initialized

global2.verify();
assert(global2.x == 20);

verify();
