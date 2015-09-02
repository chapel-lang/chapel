use myrecord;

var global2: R = new R(x=20); // constructor initialized

global2.verify();
assert(global2.x == 20);

global2.destroy();

verify();
