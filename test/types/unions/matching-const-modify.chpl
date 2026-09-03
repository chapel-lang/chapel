union U {
  var x: int;
  var y: real;
  var z: string;
  var w: int;
}

proc testConst(const u) {
  union select u {
    when x do x += 1;
    when y do y += 1.0;
    otherwise writeln("not an int or real");
  }
  writeln("in func: ", u);
}
var u = new U(x=10);
testConst(u);
