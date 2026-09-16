union U {
  var x: int;
  var y: real;
}
proc U.init=(other: U) {
  writeln("In my init=");
  union select other {
    when x do
      this.x = x;
    when y do
      this.y = y;
  }
}

var u1: U;
u1.x = 1;
var u2: U = u1;
writeln(u2);

proc main() {
  var u3: U;
  var u3a: U = u3;
  writeln(u3a);
  u3.x = 3;
  var u4: U = u3;
  writeln(u3, " ", u4);
  var u5: U = u4;
  writeln(u5);
}
