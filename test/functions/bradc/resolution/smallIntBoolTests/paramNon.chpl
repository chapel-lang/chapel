proc plus(x: int(8), y: int(8)) {
  writeln("In int version");
  writeln(x+y);
}
proc plus(x: int(16), y: int(16)) {
  writeln("In int version");
  writeln(x+y);
}
proc plus(x: int(32), y: int(32)) {
  writeln("In int version");
  writeln(x+y);
}
proc plus(x: int(64), y: int(64)) {
  writeln("In int version");
  writeln(x+y);
}

proc plus(x: uint(8), y: uint(8)) {
  writeln("In uint version");
  writeln(x+y);
}
proc plus(x: uint(16), y: uint(16)) {
  writeln("In uint version");
  writeln(x+y);
}
proc plus(x: uint(32), y: uint(32)) {
  writeln("In uint version");
  writeln(x+y);
}
proc plus(x: uint(64), y: uint(64)) {
  writeln("In uint version");
  writeln(x+y);
}

proc plus(x: real(32), y: real(32)) {
  writeln("In real version");
  writeln(x+y);
}
proc plus(x: real(32), y: real(32)) {
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

if testBoolInt then
  plus((i1 < i2), 4);
plus(ui, 1);
plus(ui, i1);
