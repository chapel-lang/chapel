config param testenum=false;

proc foo(x: int(?w), y: int(w)) {
  writeln("In int foo ", w);
}

proc foo(x: uint(?w), y: uint(w)) {
  writeln("In uint foo ", w);
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
