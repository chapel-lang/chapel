// BLC: This was a workaround for assign.chpl which doesn't seem
// to be working yet because of union prints; it's a reasonable test
// in itself, though
union U {
  var x: int;
  var y: real;
}

var u1: U;
var u2: U;

u1.x = 1;
u2 = u1;

writeln("u1.x is: ", u1.x);
writeln("u2.x is: ", u2.x);

u1.y = 2.3;
u2 = u1;

writeln("u1.y is: ", u1.y);
writeln("u2.y is: ", u2.y);

u2.x = 4;
u1.y = 5.6;
u2 = u1;

writeln("u1.y is: ", u1.y);
writeln("u2.y is: ", u2.y);

u2.y = 7.8;
u1.x = 9;
u2 = u1;

writeln("u1.x is: ", u1.x);
writeln("u2.x is: ", u2.x);
