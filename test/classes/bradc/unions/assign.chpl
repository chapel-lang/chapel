


union U {
  var x: int;
  var y: real;
}

var u1: U;
var u2: U;

u1.x = 1;
u2 = u1;

writeln("u1 is: ", u1);
writeln("u2 is: ", u2);

u1.y = 2.3;
u2 = u1;

writeln("u1 is: ", u1);
writeln("u2 is: ", u2);

u2.x = 4;
u1.y = 5.6;
u2 = u1;

writeln("u1 is: ", u1);
writeln("u2 is: ", u2);

u2.y = 7.8;
u1.x = 9;
u2 = u1;

writeln("u1 is: ", u1);
writeln("u2 is: ", u2);
