union u {
  var x: int;
  var y: int;
  var z: real;
}

var u1, u2: u;

testit();

u1.x = 33;
testit();

u2.x = 33;
testit();

u2.x = 45;
testit();

u1.x = 45;
testit();

u2.y = 45;
testit();

u1.y = 45;
testit();

u1.y = 78;
testit();

u1.z = 78.0;
testit();

u2.z = 78.0;
testit();

u2.z = 33.3;
testit();

u1.x = 33;
testit();

u2.x = 33;
testit();

u2.x = 45;
testit();

proc testit() {
  writeln(u1, " == ", u2, ": ", u1 == u2);
  writeln(u1, " != ", u2, ": ", u1 != u2);
}
