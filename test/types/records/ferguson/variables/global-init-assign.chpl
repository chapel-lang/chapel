use myrecord;

var global2: R;
global2.setup(x = 20);
global2.verify();
assert(global2.x == 20);

var global1 = global2;

global1 = global2;
global1.verify();
assert(global1.x == 20);

global1.destroy();
global2.destroy();

verify();
