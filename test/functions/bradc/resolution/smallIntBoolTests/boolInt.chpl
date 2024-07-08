config param testenum=false;

proc foo(x: int(8), y: int(8)) {
  writeln("In int foo 8");
}
proc foo(x: int(16), y: int(16)) {
  writeln("In int foo 16");
}
proc foo(x: int(32), y: int(32)) {
  writeln("In int foo 32");
}
proc foo(x: int(64), y: int(64)) {
  writeln("In int foo 64");
}

proc foo(x: uint(8), y: uint(8)) {
  writeln("In uint foo 8");
}
proc foo(x: uint(16), y: uint(16)) {
  writeln("In uint foo 16");
}
proc foo(x: uint(32), y: uint(32)) {
  writeln("In uint foo 32");
}
proc foo(x: uint(64), y: uint(64)) {
  writeln("In uint foo 64");
}

proc foo(x: int(64), y: uint(64)) {
  writeln("In int/uint foo");
}

proc foo(x: uint(64), y: int(64)) {
  writeln("In uint/int foo");
}

enum mybool {
  myfalse=0, mytrue=1
}

var i: int(8);
var ui:uint(8);
var b: bool;
var myb: mybool;

// The uint(8) / int(8) version is chosen because
// that argument is an exact match and that has
// higher priority than the "param preference" of
// the conversion from bool/enum
foo(b, i);
foo(b, ui);
if testenum {
  foo(myb, i);
  foo(myb, ui);
}
