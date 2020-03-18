union U {
  var x: int;
  var y: real;
}

var u1: U;
u1.x = 1;
var u2: U = u1;
writeln(u2);

proc main() {
  var u3: U;
  u3.x = 3;
  var u4: U = u3;
  writeln(u3, " ", u4);
  var u5: U = u4;
  writeln(u5);
}
