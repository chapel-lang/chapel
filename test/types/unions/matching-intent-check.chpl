union U {
  var x: int;
  var y: real;
  var z: string;
  var w: int;
}

proc testIn(in u) {
  union select u {
    when x do x += 1;
    when y do y += 1.0;
    otherwise writeln("not an int or real");
  }
  writeln("in func: ", u);
}
proc testRef(ref u) {
  union select u {
    when x do x += 1;
    when y do y += 1.0;
    otherwise writeln("not an int or real");
  }
  writeln("ref func: ", u);
}
proc testConstRef(const ref u) {
  union select u {
    when x do writeln("x is ", x);
    when y do writeln("y is ", y);
    otherwise writeln("not an int or real");
  }
  writeln("const ref func: ", u);
}
proc testConst(const u) {
  union select u {
    when x do writeln("x is ", x);
    when y do writeln("y is ", y);
    otherwise writeln("not an int or real");
  }
  writeln("const func: ", u);
}
proc testBlank(u) {
  union select u {
    when x do writeln("x is ", x);
    when y do writeln("y is ", y);
    otherwise writeln("not an int or real");
  }
  writeln("blank func: ", u);
}
proc testConstIn(const in u) {
  union select u {
    when x do writeln("x is ", x);
    when y do writeln("y is ", y);
    otherwise writeln("not an int or real");
  }
  writeln("const in func: ", u);
}

var u = new U(x=10);
testIn(u);
writeln("after in func: ", u);
testRef(u);
writeln("after ref func: ", u);
testConstRef(u);
writeln("after const ref func: ", u);
testConst(u);
writeln("after const func: ", u);
testBlank(u);
writeln("after blank func: ", u);
testConstIn(u);
writeln("after const in func: ", u);
