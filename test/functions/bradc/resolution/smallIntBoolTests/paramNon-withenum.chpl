proc plus(x: int(?w), y: int(w)) {
  writeln("In int version");
  writeln(x+y);
}

proc plus(x: uint(?w), y: uint(w)) {
  writeln("In uint version");
  writeln(x+y);
}

proc plus(x: real(?w), y: real(w)) {
  writeln("In real version");
  writeln(x+y);
}


proc plus(x: uint(64), param y: int(64)) {
  writeln("In uint/param int version");
  if (x < 0) then writeln("compilerError: uint/param int version"); else writeln(x+y);
}

proc plus(param x: int(64), y: uint(64)) {
  writeln("In param int/uint version");
  if (x < 0) then writeln("compilerError: param int/uint version"); else writeln(x+y);
}

proc plus(x: int(64), param y: int(64)) {
  writeln("In int/param int version");
  if (x < 0) then writeln("compilerError: uint/param int version"); else writeln(x+y);
}

proc plus(param x: int(64), y: int(64)) {
  writeln("In param int/int version");
  if (x < 0) then writeln("compilerError: param int/uint version"); else writeln(x+y);
}



proc plus(x: uint(64), y: int(64)) {
  writeln("In uint/int version");
  writeln("compilerError: uint/int op error");
}

proc plus(x: int(64), y: uint(64)) {
  writeln("In int/uint version");
  writeln("compilerError: uint/int op error");
}


config var i1 = 2;
config var i2 = 3;

config var ui: uint(64) = 4;

config param testBoolInt = true;

enum mybool {
  myfalse=0, mytrue=1
}

var mb: mybool = mybool.mytrue;

if testBoolInt then
  plus(mb, 4);
plus(ui, 1);
plus(ui, i1);
