union U {
  var x: int;
  var y: real;
  var z: string;
  var w: int;
}
record R { var u: U; }
proc test3() {
  var uR = new R();
  writeln("union in record");
  union select uR.u { // doesn't work, can be worked around with ref local
    when x do x += 1;
    when y do y += 1.0;
    otherwise writeln("not an int or real");
  }
  writeln(uR);
  uR.u.x = 10;
  writeln(uR);
  union select uR.u {
    when x do x += 1;
    when y do y += 1.0;
    otherwise writeln("not an int or real");
  }
  writeln(uR);
}
test3();
