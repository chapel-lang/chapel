use myrecord;

var global1: R; // default initialized
global1.verify();

var global2: R;
global2.init(x = 20);
global2.verify();
assert(global2.x == 20);

global1.destroy();
global2.destroy();

verify();
